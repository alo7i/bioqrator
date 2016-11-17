class AnnotationsController < ApplicationController
  before_filter :authenticate_user!

  # GET /annotations
  # GET /annotations.json
  def index
    @collection = current_user.collections
          .includes({:documents => [:passages, {:annotations => :locations}, 
            {:relations => {:references => [{:child => :locations}]}}]})
          .find(params[:collection_id])

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @annotations }
    end
  end

  # GET /annotations/1
  # GET /annotations/1.json
  def show
    @annotation = Annotation.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @annotation }
    end
  end

  # GET /annotations/new
  # GET /annotations/new.json
  def new
    @annotation = Annotation.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @annotation }
    end
  end

  # GET /annotations/1/edit
  def edit
    @annotation = Annotation.find(params[:id])
  end


  # POST /annotations
  # POST /annotations.json
  def create
    if InfoNode.duplicate_key?(params[:info_types])
      redirect_to :back, :alert => "Key in the additional information shoud be unique"
      return
    end

    @atext = params[:text] unless params[:text].nil?
    @representative_name = nil
    unless params[:representative_name].nil? 
      name = params[:representative_name].strip
      @representative_name = name if !name.blank? && name != @atext
    end
    logger.debug("#{@atext} == 'f a' ? #{@text == 'f a'}")
    @aid = params[:aid].strip unless params[:aid].nil?

    @offset = params[:offset]
    @length = params[:length]
    @type = params[:type]
    @info_nodes = {}
    (0...params[:info_types].size).each do |i|
      key = params[:info_types][i]
      value = params[:info_values][i]
      if !key.nil? && !key.strip.blank? && !value.nil? && !value.strip.blank?
        @info_nodes[key] = value
      end
    end
    
    logger.debug("IN controller#create '#{@atext}'")
    begin
      Node.transaction do 
        @collection = current_user.collections.find(params[:collection_id])
        raise "Cannot find the collection No. #{params[:collection_id]}" if @collection.nil?
        
        @document = @collection.documents.find_by_pmid(params[:document_id])
        raise "Cannot find the document No. #{params[:document_id]}" if @document.nil?
          
        @aid = @document.create_new_annotation_id if @aid.blank?

        @annotation = @document.create_annotation(@aid, @atext, @representative_name, @type, @info_nodes, @offset.to_i, @length.to_i)

        unless params[:find_all].nil?
          @annotation.add_all
        end

        respond_to do |format|
          format.html { redirect_to "/collections/#{@collection.id}/documents/#{@document.pmid}", :notice => 'Annotation was successfully created.' }
          format.json { render :json => @annotation, :status => :created, :location => @annotation }
        end
      end
    end
  end

  def import
    
  end

  # PUT /annotations/1
  # PUT /annotations/1.json
  def update
    @annotation = Node.find(params[:id])

    if InfoNode.duplicate_key?(params[:info_types])
      redirect_to :back, :alert => "Key in the additional information shoud be unique"
      return
    end

    @collection = current_user.collections.find(params[:collection_id])      
    if @annotation.nil? || @collection.nil? || @annotation.document.collection.id != @collection.id
      redirect_to :back, :alert => "Cannot find an annotation"
      return
    end

    unless params[:aid].nil?
      @annotation.node_id = params[:aid].strip
    end
    @annotation.representative_name = nil
    unless params[:representative_name].nil? 
      name = params[:representative_name].strip
      @annotation.representative_name = name if !name.blank? && name != params[:text]
    end

    @aid = @annotation.document.create_new_annotation_id if @aid.blank?
    @annotation.entity_type = params[:type]

    keyset = []
    (0...params[:info_types].size).each do |i|
      key = params[:info_types][i]
      value = params[:info_values][i]
      if !key.nil? && !key.strip.blank? && !value.nil? && !value.strip.blank?
        keyset << key
        @annotation.set_info_node(key, value)
      end
    end

    @annotation.remove_info_node_except(keyset)

    respond_to do |format|
      if @annotation.save
        format.html { redirect_to "/collections/#{@annotation.document.collection.id}/documents/#{@annotation.document.pmid}", notice: 'Annotation was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { redirect_to :back, :alert => "Cannot update annotation" }
        format.json { render json: @annotation.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /annotations/1
  # DELETE /annotations/1.json
  def destroy
    @annotation = Node.find(params[:id])
    @collection = current_user.collections.find(params[:collection_id])
      
    if @annotation.nil? || @collection.nil? || @annotation.document.collection.id != @collection.id
      redirect_to :back, :alert => "Cannot find an annotation"
      return
    end

    @document = @annotation.document

    respond_to do |format|
      if @annotation.destroy
        format.html { redirect_to "/collections/#{@document.collection.id}/documents/#{@document.pmid}", :notice => "Annotation was successfully deleted." }
        format.json { head :no_content }
      else
        format.html { redirect_to :back, :alert => "Cannot delete annotation: #{@annotation.errors.full_messages}" }
        format.json { render json: @annotation.errors, status: :unprocessable_entity }
      end
    end
  end
end
