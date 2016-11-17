class RecommendersController < ApplicationController
  before_filter :authenticate_user!
  # GET /recommenders
  # GET /recommenders.json
  def index
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommenders = @collection.recommenders.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @recommenders }
    end
  end

  # GET /recommenders/1
  # GET /recommenders/1.json
  def show
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @recommender }
    end
  end

  # GET /recommenders/new
  # GET /recommenders/new.json
  def new
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @recommender }
    end
  end

  # GET /recommenders/1/edit
  def edit
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.find(params[:id])
  end

  # POST /recommenders
  # POST /recommenders.json
  def create
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.new(params[:recommender])

    respond_to do |format|
      if @recommender.save
        format.html { redirect_to collection_recommenders_path(@collection), notice: 'Recommender was successfully created.' }
        format.json { render json: @recommender, status: :created, location: @recommender }
      else
        format.html { render action: "new" }
        format.json { render json: @recommender.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /recommenders/1
  # PUT /recommenders/1.json
  def update
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.find(params[:id])

    respond_to do |format|
      if @recommender.update_attributes(params[:recommender])
        format.html { redirect_to collection_recommenders_path(@collection), notice: 'Recommender was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @recommender.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /recommenders/1
  # DELETE /recommenders/1.json
  def destroy
    @collection = current_user.collections.find(params[:collection_id]) 
    @recommender = @collection.recommenders.find(params[:id])
    @recommender.destroy

    respond_to do |format|
      format.html { redirect_to collection_recommenders_path(@collection) }
      format.json { head :no_content }
    end
  end
end
