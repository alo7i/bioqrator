class SaveQueriesController < ApplicationController
  # GET /save_queries
  # GET /save_queries.json
  def index
    @collection = current_user.collections.find(params[:collection_id]) 
    @save_queries = @collection.save_queries

    if params[:id].nil?
      @query = @save_queries.first unless @save_queries.empty?
    else
      @query = @save_queries.find(params[:id])
    end

    @pageSize = 12
    @page = params[:page] || 1
    @page = @page.to_i
    @page = 1 if @page < 1

    @intervals = []

    unless @query.nil?
      logger.debug(@query.inspect)
      @intervals = SaveQueryCache.generate_intervals_for_pages(@query.duration, @page, @pageSize)
      logger.debug(@intervals)
      @caches = @query.get_caches_in_intervals(@intervals)
    end

    logger.debug(@intervals)

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @save_queries }
    end
  end

  def refresh
    @save_query = SaveQuery.find(params[:id])
    @cache = @save_query.query(params[:date], true)
    respond_to do |format|
      format.json { render json: @cache}
    end
  end
  # GET /save_queries/1
  # GET /save_queries/1.json
  def show
    @collection = current_user.collections.find(params[:collection_id]) 
    @save_queries = @collection.save_queries

    if params[:id].nil?
      @query = @save_queries.first unless @save_queries.empty?
    else
      @query = @save_queries.find(params[:id])
    end

    @term = @query.term

    @page = params[:page] || 1
    @pagesize = params[:pagesize] || 10
    @refresh = params[:refresh] || false
    params[:refresh] = false

    @order = params[:order] || "score"
    @orderstr = (@order == "pubdate") ? "Publication Date" : "PIE Score"

    @date = params[:date]
    @startDate = (@date.split(":")[0]).to_time
    @intervals = SaveQueryCache.generate_intervals_for_date(@query.duration, @startDate, 12)
    @caches = @query.get_caches_in_intervals(@intervals)

    @cache = @query.query(@date, @refresh)
    if @order == "score"
      @pmids = @cache.save_query_docs.order("score DESC")
    else
      @pmids = @cache.save_query_docs.order("pubdate DESC")
    end

    @pmids = @pmids.page(@page).per(@pagesize)
    @count = @cache.save_query_docs_count

    logger.debug(@intervals)
  end

  # GET /save_queries/new
  # GET /save_queries/new.json
  def new
    @save_query = SaveQuery.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @save_query }
    end
  end

  # GET /save_queries/1/edit
  def edit
    @save_query = SaveQuery.find(params[:id])
  end

  # POST /save_queries
  # POST /save_queries.json
  def create
    @collection = current_user.collections.find(params[:collection_id]) 
    @save_query = @collection.save_queries.new(params[:save_query])

    respond_to do |format|
      if @save_query.save
        format.html { redirect_to collection_save_queries_path(@collection), notice: 'Save query was successfully created.' }
        format.json { render json: @save_query, status: :created, location: @save_query }
      else
        format.html { redirect_to collection_save_queries_path(@collection), alert: "Error: #{ @save_query.errors.full_messages.join('/')}" }
        format.json { render json: @save_query.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /save_queries/1
  # PUT /save_queries/1.json
  def update
    @collection = current_user.collections.find(params[:collection_id]) 
    @save_query = @collection.save_queries.find(params[:id])

    respond_to do |format|
      if @save_query.update_attributes(params[:save_query])
        format.html { redirect_to @save_query, notice: 'Save query was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @save_query.errors, status: :unprocessable_entity }
      end
    end
  end



  # DELETE /save_queries/1
  # DELETE /save_queries/1.json
  def destroy
    @collection = current_user.collections.find(params[:collection_id]) 
    @save_query = @collection.save_queries.find(params[:id])
    @save_query.destroy

    respond_to do |format|
      format.html { redirect_to collection_save_queries_path(@collection) }
      format.json { head :no_content }
    end
  end
end
