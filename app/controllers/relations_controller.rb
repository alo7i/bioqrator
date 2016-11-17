class RelationsController < ApplicationController
  # GET /relations
  # GET /relations.json
  def index
    @collection = current_user.collections
          .includes({:documents => [:passages, {:annotations => :locations}, 
            {:relations => {:references => [{:child => :locations}]}}]})
          .find(params[:collection_id])

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @relations }
    end
  end

  # GET /relations/1
  # GET /relations/1.json
  def show
    @relation = Relation.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @relation }
    end
  end

  # GET /relations/new
  # GET /relations/new.json
  def new
    @relation = Relation.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @relation }
    end
  end

  # GET /relations/1/edit
  def edit
    @relation = Relation.find(params[:id])
  end

  # POST /relations
  # POST /relations.json
  def create    
    if InfoNode.duplicate_key?(params[:info_types])
      redirect_to :back, :alert => "Key in the additional information shoud be unique"
      return
    end

    unless params[:rid].nil?
      @rid = params[:rid].strip
    end

    @type = params[:type] 

    @refs = []

    params[:refids].each_with_index do |ref_id, idx|
      @refs << {:ref_id => ref_id, :role => params[:labels][idx]}
    end unless params[:refids].nil?

    if @refs.empty?
      redirect_to :back, :alert => "Please add references for a new relation"
      return
    end

    @info_nodes = {}
    (0...params[:info_types].size).each do |i|
      key = params[:info_types][i]
      value = params[:info_values][i]
      if !key.nil? && !key.strip.blank? && !value.nil? && !value.strip.blank?
        @info_nodes[key] = value
      end
    end

    begin
      @collection = current_user.collections.find(params[:collection_id])
      raise "Cannot find the collection No. #{params[:collection_id]}" if @collection.nil?
      
      @document = @collection.documents.find_by_pmid(params[:document_id])
      raise "Cannot find the document No. #{params[:document_id]}" if @document.nil?
        
      @rid = @document.create_new_relation_id if @rid.blank?
  
      @relation = @document.create_relation(@rid, @type, @refs, @info_nodes)

      respond_to do |format|
        format.html { redirect_to "/collections/#{@collection.id}/documents/#{@document.pmid}?tab=relation", :notice => 'Relation was successfully created.' }
        format.json { render :json => @relation, :status => :created, :location => @relation }
      end
    rescue Exception => e
      logger.debug(e.backtrace)
      respond_to do |format|
        format.html { redirect_to :back, :alert => e.message}
        format.json { render json: @relation.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /relations/1
  # PUT /relations/1.json
  def update

    if InfoNode.duplicate_key?(params[:info_types])
      redirect_to :back, :alert => "Key in the additional information shoud be unique"
      return
    end

    @relation = Node.find(params[:id])
    @collection = current_user.collections.find(params[:collection_id])
    if @relation.nil? || @collection.nil? || @relation.document.collection.id != @collection.id
      redirect_to :back, :alert => "Cannot find an relation"
      return
    end 

    @document = @relation.document
    @collection = @document.collection

    unless params[:rid].nil?
      @rid = params[:rid].strip
    end

    @rid = @document.create_new_relation_id if @rid.blank?
    @relation.entity_type = params[:type] 
    @refs = []

    params[:refids].each_with_index do |ref_id, idx|
      @refs << {:ref_id => ref_id, :role => params[:labels][idx]}
    end unless params[:refids].nil?

    if @refs.empty?
      redirect_to :back, :alert => "Please add references for a new relation"
      return
    end
    
    @relation.update_references(@refs)

    keyset = []
    (0...params[:info_types].size).each do |i|
      key = params[:info_types][i]
      value = params[:info_values][i]
      if !key.nil? && !key.strip.blank? && !value.nil? && !value.strip.blank?
        keyset << key
        @relation.set_info_node(key, value)
      end
    end

    @relation.remove_info_node_except(keyset)


    respond_to do |format|
      if @relation.save
        format.html { redirect_to "/collections/#{@collection.id}/documents/#{@document.pmid}?tab=relation",  notice: 'Relation was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { redirect_to :back, :alert => "Cannot update relation: #{@relation.errors.full_messages}" }
        format.json { render json: @relation.errors, status: :unprocessable_entity }
      end
    end

      
  end

  # DELETE /relations/1
  # DELETE /relations/1.json
  def destroy
    @relation = Node.find(params[:id])
    @collection = current_user.collections.find(params[:collection_id])

    if @relation.nil? || @collection.nil? || @relation.document.collection.id != @collection.id
      redirect_to :back, :alert => "Cannot find an relation"
      return
    end

    @document = @relation.document
    @collection = @document.collection

    logger.debug @relation.errors.inspect
    respond_to do |format|
      if @relation.destroy
        format.html { redirect_to "/collections/#{@collection.id}/documents/#{@document.pmid}?tab=relation", :notice => "Relation was successfully deleted." }
        format.json { head :no_content }
      else
        format.html { redirect_to :back, :alert => "Cannot delete relation: #{@relation.errors.full_messages}" }
        format.json { render json: @relation.errors, status: :unprocessable_entity }
      end
    end
  end
end
