class EntityTypesController < ApplicationController
  # GET /entity_types
  # GET /entity_types.json

  def index
    @collection = current_user.collections.find(params[:collection_id])

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @entity_types }
    end
  end

  # GET /entity_types/1
  # GET /entity_types/1.json
  def show
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = @collection.entity_types.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @entity_type }
    end
  end

  # GET /entity_types/new
  # GET /entity_types/new.json
  def new
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = EntityType.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @entity_type }
    end
  end

  # GET /entity_types/1/edit
  def edit
    session[:edit_referrer] = request.env["HTTP_REFERER"]
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = @collection.entity_types.find(params[:id])
  end

  # POST /entity_types
  # POST /entity_types.json
  def create
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = @collection.entity_types.new(params[:entity_type])

    respond_to do |format|
      if @entity_type.save
        format.html { redirect_to collection_entity_types_path(@collection), notice: 'Type was successfully created.' }
        format.json { render json: @entity_type, status: :created, location: @entity_type }
      else
        format.html { render action: "new" }
        format.json { render json: @entity_type.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /entity_types/1
  # PUT /entity_types/1.json
  def update
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = @collection.entity_types.find(params[:id])
  
    if session[:edit_referrer].nil? 
      success_path = collection_entity_types_path(@collection)
    else
      success_path = session[:edit_referrer]
      session[:edit_referrer] = nil
    end
    
    respond_to do |format|
      if @entity_type.update_attributes(params[:entity_type])
        format.html { redirect_to  success_path, notice: 'Type was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @entity_type.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /entity_types/1
  # DELETE /entity_types/1.json
  def destroy
    @collection = current_user.collections.find(params[:collection_id])
    @entity_type = @collection.entity_types.find(params[:id])
    @entity_type.destroy
    logger.debug(@entity_type.errors.inspect)
    respond_to do |format|
      if @entity_type.errors.size == 0
        format.html { redirect_to collection_entity_types_path(@collection)}
        format.json { head :no_content }
      else
        format.html { redirect_to collection_entity_types_path(@collection), alert: "Cannot be destroyed: #{@entity_type.errors[:base].to_s}"}
        format.json { head :no_content }
      end
    end
  end
end
