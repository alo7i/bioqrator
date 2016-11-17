class InfoNodesController < ApplicationController
  # GET /info_nodes
  # GET /info_nodes.json
  def index
    @info_nodes = InfoNode.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @info_nodes }
    end
  end

  # GET /info_nodes/1
  # GET /info_nodes/1.json
  def show
    @info_node = InfoNode.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @info_node }
    end
  end

  # GET /info_nodes/new
  # GET /info_nodes/new.json
  def new
    @info_node = InfoNode.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @info_node }
    end
  end

  # GET /info_nodes/1/edit
  def edit
    @info_node = InfoNode.find(params[:id])
  end

  # POST /info_nodes
  # POST /info_nodes.json
  def create
    @info_node = InfoNode.new(params[:info_node])

    respond_to do |format|
      if @info_node.save
        format.html { redirect_to @info_node, notice: 'Info node was successfully created.' }
        format.json { render json: @info_node, status: :created, location: @info_node }
      else
        format.html { render action: "new" }
        format.json { render json: @info_node.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /info_nodes/1
  # PUT /info_nodes/1.json
  def update
    @info_node = InfoNode.find(params[:id])

    respond_to do |format|
      if @info_node.update_attributes(params[:info_node])
        format.html { redirect_to @info_node, notice: 'Info node was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @info_node.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /info_nodes/1
  # DELETE /info_nodes/1.json
  def destroy
    @info_node = InfoNode.find(params[:id])
    @info_node.destroy

    respond_to do |format|
      format.html { redirect_to info_nodes_url }
      format.json { head :no_content }
    end
  end
end
