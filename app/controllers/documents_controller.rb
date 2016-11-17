# encoding: utf-8
require 'bio'

class DocumentsController < ApplicationController
  before_filter :authenticate_user!
  
  def show
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    @document = @collection.documents.includes(
      [
        :passages, {:annotations => [:locations, :passage, :info_nodes]}, 
        {:relations => [:references, :info_nodes] }
      ]
      ).where("pmid = ? ", params[:id]).first 

    @prev_document = @collection.documents.last
    @next_document = @collection.documents.first
    pp = nil
    @collection.documents.each do |d|
      if d == @document && !pp.nil?
        @prev_document = pp
      end

      if pp == @document 
        @next_document = d
        break
      end

      pp = d
    end

    @document.passages.each do |p|
      p.ptext = p.ptext.gsub("\u00a0", ' ')
    end

    @tab = params[:tab] 
    if @collection.nil? || @document.nil?
      flash[:alert] = "Collection or Document does not exist"
      redirect_to :back
    else
      render
    end  
  end

  def load_pie
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = @collection.documents.find_by_pmid(params[:id])
    @tab = params[:tab] 
    if @collection.nil? || @document.collection_id != @collection.id
      flash[:alert] = "Collection or Document does not exist"
      redirect_to :back
      return
    else
      @document.import_pie_entities
      flash[:notice] = "PIE annotations were successfully imported"
      redirect_to :back
      return
    end  
  end

  def import_pie
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = @collection.documents.find_by_pmid(params[:id])
    @checks = params[:check]
    @offsets = params[:offset]
    @names = params[:name]
    @types = params[:type]

    if @checks.nil? || @checks.empty? || 
          @checks.select{|k, v| v == '1'}.size == 0 || 
          @checks.size != @offsets.size || 
          @checks.size != @names.size ||
          @checks.size != @types.size
      flash[:alert] = "No item selected. Please select items to import"
      redirect_to :back
      return
    end
    if @collection.nil? || @document.collection_id != @collection.id
      flash[:alert] = "Collection or Document does not exist"
      redirect_to :back
      return
    else
      @checks.each do |k, v|
        if v == '1'
          @document.import_pie_annotation(@offsets[k], URI.unescape(@names[k]), @types[k]);
        end
      end
      flash[:notice] = "PIE annotations were successfully imported"
      redirect_to :back
      return
    end  
  end

  def create
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @new_documents = @collection.add_documents(params[:pmid], params[:score]) unless params[:pmid].nil?
    render :layout => nil
  end

  def done
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = @collection.documents.find_by_pmid(params[:id])

    if @collection.nil? || @document.collection_id != @collection.id
      render :text => "Not found", :status => 404
    else
      @document.done = params[:done] || @document.done
      @document.curatable = params[:curatable] || @document.curatable
      if @document.save
        render :layout => nil
      else
        render :text => "error", :status => 500
      end
    end    
  end

  def check
    update_date = Date.strptime(params[:update], "%Y-%m-%d")
    DocumentCache.expire(params[:pmid], nil, update_date)
    render :text => "ok"
  end

  def refetch
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = @collection.documents.find_by_pmid(params[:id])
    @document.destroy_bulk(false)
    @document.fetch(nil, true) unless @document.nil?
    render :text => "ok"
  end

  def new
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @type = "query"
    unless params[:upload].nil?
      @filename = params[:upload].original_filename
      content = params[:upload].read
      @ids = content.scan(/\d+/).map{|d| d.to_i }.uniq.select{|d| d > 0}
      @type = "upload"
    end

    if @type == "query"

      params[:pagesize] ||= 20
      params[:page] ||= 1
      
      @term = params[:term]
      @date = params[:date]

      @save_query = @collection.save_queries.where("term = ?", @term).first
      @order = params[:order] || "score"
      @orderstr = (@order == "pubdate") ? "Publication Date" : "PIE Score"
      @pagesize = params[:pagesize]


      if @save_query.nil?
        logger.debug("NIL")
        @cache = SaveQueryCache.fetch(@term, @date)
      else
        logger.debug("NOT NIL")
        @cache = @save_query.query(@date, false)
      end
      if @order == "score"
        @pmids = @cache.save_query_docs.order("score DESC")
      else
        @pmids = @cache.save_query_docs.order("pubdate DESC")
      end

      @pmids =  @pmids.page(params[:page]).per(params[:pagesize])
      @count = @cache.save_query_docs_count
    end
  end

  # DELETE /collections/1
  # DELETE /collections/1.json
  def destroy
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = @collection.documents.find_by_id(params[:id])
    @document.destroy

    respond_to do |format|
      format.html { redirect_to @collection, notice: 'Document was successfully removed.'}
      format.json { head :no_content }
    end
  end

  def fetch
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @document = DocumentCache.fetch(params[:id])
    render :layout => nil
  end

  def query
    params[:pagesize] ||= 20
    params[:page] ||= 1
    begin
      @collection = baseCollections.find(params[:collection_id]) 
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    
    @term = params[:term]
    @date = params[:date]

    @save_query = @collection.save_queries.where("term = ?", @term).first
    

    @order = params[:order] || "score"
    @orderstr = (@order == "pubdate") ? "Publication Date" : "PIE Score"
    @pagesize = params[:pagesize]


    if @save_query.nil?
      @cache = SaveQueryCache.fetch(@term, @date)
    else
      @cache = @save_query.query(@date, false)
    end
    if @order == "score"
      @pmids = @cache.save_query_docs.order("score DESC")
    else
      @pmids = @cache.save_query_docs.order("pubdate DESC")
    end

    @pmids =  @pmids.page(params[:page]).per(params[:pagesize])
    @count = @cache.save_query_docs_count

    # else 
    #   if @source == "pie"
    #     @order = params[:order] || "score"
    #   else
    #     @order = params[:order] || "pubdate"
    #   end

    #   @orderstr = (@order == "pubdate") ? "Publication Date" : "PIE Score"

    #   @pagesize = params[:pagesize]

    #   PubmedQuery.transaction do 
    #     query = PubmedQuery.esearch(params[:term], @date)

    #     @count = query.count

    #     if @source == "pie"
    #       @pmids = PubmedQueryPmid.where("pie_score is not null and pubmed_query_id = ?", query.id)
    #     else
    #       @pmids = PubmedQueryPmid.where("pubmed_query_id = ?", query.id)
    #     end

    #     if @order == "score"
    #       @pmids = @pmids.order("pie_score DESC")
    #     else
    #       @pmids = @pmids.order("pubmed_order")
    #     end

    #     @pmids =  @pmids.page(params[:page]).per(params[:pagesize])
    #   end
    # end

    logger.debug(@pmids.inspect)
    render :layout => nil
  end

end
