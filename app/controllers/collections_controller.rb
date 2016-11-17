require "csv"
class CollectionsController < ApplicationController
  before_filter :authenticate_user!
  
  # GET /collections
  # GET /collections.json
  def index
    @collections = baseCollections.includes(:owner).order(:name).all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @collections }
    end
  end

  # GET /collections/1
  # GET /collections/1.json
  def show
    @pmids = params[:pmids]
    begin
      @collection = baseCollections
          .includes({:documents => [:passages, {:annotations => :locations}, 
            {:relations => {:references => [{:node => :locations}]}}]})
          .find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    if @pmids.nil?
      @documents = @collection.documents.order("ppi_score DESC").includes(:passages, {:annotations => :locations}, 
            {:relations => {:references => [{:node => :locations}]}})
    else
      @documents = @collection.documents.where("pmid in (?)", @pmids.split(",")).order("ppi_score DESC").includes(:passages, {:annotations => :locations}, 
            {:relations => {:references => [{:node => :locations}]}})
    end
    filename = (params[:filename] && params[:filename].strip) 
    filename = @collection.name if filename.blank?
    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @collection }
      format.xml do
        response.headers['Content-Disposition'] = "attachment; filename=\"#{filename}.xml\""
        render :layout => nil
      end
      format.csv do 
        response.headers['Content-Disposition'] = "attachment; filename=\"#{filename}.csv\""
        render :layout => nil
      end        
    end
  end

  def remove_documents
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @collection.remove_documents
    redirect_to @collection, :notice => "Successfully removed."
  end

  def remove_annotations
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @collection.remove_annotations
    redirect_to @collection, :notice => "Successfully removed."
  end

  def share
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @collection }
      format.xml {render :layout => nil}
    end
  end
  
  def mark_negative
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @collection.mark_negative(params[:pmid])
    render :text => "ok"
  end

  def unmark_negative
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
    @collection.unmark_negative(params[:pmid])
    render :text => "ok"
  end

  def add_share
    begin
      @collection = current_user.own_collections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    @user = User.find_by_email(params[:email])
    if @user.nil? 
      redirect_to :back, :alert => "Not exist user with email '#{params[:email]}'"
      return
    end

    user = @collection.users.find_by_id(@user.id)
    unless user.nil? 
      redirect_to :back, :alert => "Already shared to user '#{user.name_or_email}'"
      return
    end

    ret = @collection.user_collections.create({:user_id => @user.id})
    if ret
      redirect_to share_collection_path(@collection), :notice => "Collection was successfully shared to #{@user.name_or_email}"
    else
      redirect_to share_collection_path(@collection), :alert => "Failed to share"
    end
  end

  def remove_share
    begin
      @collection = current_user.own_collections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    
    user = @collection.user_collections.find_by_user_id(params[:user])
    if user.nil? 
      redirect_to :back, :alert => "Collection is not shared to the user #{params[:user]}"
      return
    end
    

    if user.destroy
      redirect_to share_collection_path(@collection), :notice => "Collection was successfully updated"
    else
      redirect_to share_collection_path(@collection), :alert => "Failed to remove the shared user"
    end
  end

  # GET /collections/new
  # GET /collections/new.json
  def new
    @collection = current_user.own_collections.new
    @collection.s_date = Time.now.strftime("%Y-%m-%d")
    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @collection }
    end
  end

  # GET /collections/1/edit
  def edit
    begin
      @collection = baseCollections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end
  end

  # POST /collections
  # POST /collections.json
  def create
    @collection = current_user.own_collections.new(params[:collection])
    @collection.owner = current_user
    @collection.s_date = Time.now
    respond_to do |format|
      if @collection.save
        format.html { redirect_to @collection, notice: 'Collection was successfully created.' }
        format.json { render json: @collection, status: :created, location: @collection }
      else
        format.html { render action: "new" }
        format.json { render json: @collection.errors, status: :unprocessable_entity }
      end
    end
  end


  # PUT /collections/1
  # PUT /collections/1.json
  def update
    begin
      @collection = current_user.own_collections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    respond_to do |format|
      if @collection.update_attributes(params[:collection])
        format.html { redirect_to @collection, notice: 'Collection was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @collection.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /collections/1
  # DELETE /collections/1.json
  def destroy
    begin
      @collection = current_user.own_collections.find(params[:id])
    rescue ActiveRecord::RecordNotFound
      redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
      return
    end

    @collection.destroy_bulk

    respond_to do |format|
      format.html { redirect_to collections_url, notice: 'Collection was successfully removed.'}
      format.json { head :no_content }
    end
  end

  def upload
    if !params[:id].nil?
      logger.debug("PARMA!!! #{params.inspect}")
      begin
        @collection = baseCollections.find(params[:id])
      rescue ActiveRecord::RecordNotFound
        redirect_to collections_url, :alert => "Not exist collection or you are not allowed to see."
        return
      end
    end

    begin
      logger.debug("PARMA!!! #{params.inspect} #{current_user.own_collections.inspect}")
      unless params[:upload].nil?
        ActiveRecord::Base.transaction do
          logger.debug("+========+++++++++++++++++++++++++++++++++++  ")
          @collection = current_user.own_collections.create({:name => params[:upload].original_filename}) if @collection.nil?
          if !@collection.save
            logger.debug(@collection.errors.inspect)
          end  
          logger.debug("#{@collection.inspect} COLLECTION DEBUG ======================")
          @collection.load_from_xml(params[:upload])
        end

        flash[:notice] = "Save collection"

        respond_to do |format|
          unless @collection.nil?
            format.html { redirect_to @collection, notice: 'Collection was successfully updated.' }
            format.json { render json: @collection, status: :created, location: @collection }
          else
            format.html { redirect_to :back, alert: 'Error' }
            format.json { render json: @collection.errors, status: :unprocessable_entity }
          end
        end
      else
        redirect_to :back, alert: 'Please select a file'
      end
    rescue => e
      logger.debug(e.message)
      logger.debug(e.backtrace)
      redirect_to :back, alert: 'Wrong file format: Does this file have a valid BioC format?'
    end
  end


end
