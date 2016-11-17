require 'bio'

class Collection < ActiveRecord::Base
  attr_accessible :key, :name, :owner_id, :s_date, :source, :documents_count

  has_many :documents, :dependent => :destroy, :order => "ppi_score DESC"
  has_many :negative_documents, :dependent => :destroy
  has_many :recommenders, :dependent => :destroy
  
  has_many :entity_types, :dependent => :destroy, :order => "name asc"
  has_many :e_entity_types, :class_name => "EntityType", :conditions => "for_entity = 1" 
  has_many :r_entity_types, :class_name => "EntityType", :conditions => "for_relation = 1"

  has_many :info_nodes, :dependent => :destroy
  belongs_to :owner, :class_name => "User", :foreign_key => :owner_id
  
  has_many :user_collections
  has_many :users, :through => :user_collections
  has_many :save_queries, :dependent => :destroy
  validates :name, :presence => true
 
  after_create do |collection|
    collection.user_collections.create({:user_id => collection.owner_id})
    collection.find_or_create_entity_type('Protein', true, false)
    collection.find_or_create_entity_type('Interact', false, true)
    collection.find_or_create_entity_type('Negated_Interact', false, true)
  end 

  def own?(user)
    return owner_id == user.id
  end

  def remove_documents
    Collection.transaction do
      self.documents.each do |d|
        d.destroy_bulk
      end
      Document.delete_all("collection_id = #{self.id}")
      NegativeDocument.delete_all("collection_id = #{self.id}")
      Collection.reset_counters(self.id, :documents)
    end
  end

  def destroy_bulk
    self.documents.each do |d|
      d.destroy_bulk
    end
    Document.delete_all("collection_id = #{self.id}")
    NegativeDocument.delete_all("collection_id = #{self.id}")
    InfoNode.delete_all("collection_id = #{self.id}")
    Recommender.delete_all("collection_id = #{self.id}")
    EntityType.delete_all("collection_id = #{self.id}")
    self.save_queries.each do |sq|
      sq.destroy_bulk
    end
    SaveQuery.delete_all("collection_id = #{self.id}")
    self.delete
  end

  def remove_annotations
    self.documents.each {|d| d.remove_annotations }
  end

  def positive?(pmid)
    doc = self.documents.where("pmid = ?", pmid).first
    return !doc.nil? 
  end

  def negative?(pmid)
    doc = self.negative_documents.where("pmid = ?", pmid).first
    return !doc.nil? 
  end

  def mark_negative(pmid) 
    doc = self.negative_documents.where("pmid = ?", pmid).first
    self.negative_documents.create({:pmid => pmid}) if doc.nil?
  end

  def unmark_negative(pmid) 
    doc = self.negative_documents.where("pmid = ?", pmid).first
    unless doc.nil?
      doc.destroy
    end
  end

  def add_documents(pmids, ppi_scores)
    ret = []
    old_pmids = documents.map{|d| d.pmid} unless documents.blank?
    pmids = pmids - old_pmids if !old_pmids.nil?
    
    pmids.each_with_index do |pmid, index|
      ret << find_or_create_document(pmid, ppi_scores[index])
      self.unmark_negative(pmid)
    end
    
    ret
  end

  def find_or_create_document(pmid, ppi_score = nil)
    logger.debug("#{pmid} FIND OR CREATE DOCUMENT")
    document = self.documents.find_by_pmid(pmid)
    if document.nil?
      document = self.documents.create({:pmid => pmid, :ppi_score => ppi_score, :curatable => true})
      document.fetch
    end

    return document
  end
  
  def has_annotation?
    return false if self.documents.nil? || self.documents.empty?
    self.documents.each do |d|
      return true unless d.annotations.blank?
    end
    return false
  end

  def find_or_create_entity_type(type, for_entity = true, for_relation = true)
    entity_type = self.entity_types.where("name = ? ", type).first
    if entity_type.nil?
      entity_type = self.entity_types.create({:name => type, :for_entity => for_entity, :for_relation => for_relation})
    end
    return entity_type
  end

  def has_relations?
    return false if self.documents.nil? || self.documents.empty?
    self.documents.each do |d|
      return true unless d.relations.blank?
    end
    return false
  end

  def load_from_xml(file) 
    xml_doc  = Nokogiri::XML(file) do |config|
      config.noent.nonet.noblanks
    end

    collection = xml_doc.at_xpath("//collection")
    if collection.nil?
      raise 'Wrong format'
    end

    source = collection.at_xpath("source")
    date = collection.at_xpath("date")
    key = collection.at_xpath("key")

    self.source = source.content unless source.nil?
    self.s_date = date.content unless date.nil?
    self.key = key.content unless key.nil?
    self.save 
    collection.xpath("infon").each do |infon|
      key = infon["key"]
      value = infon.content
      set_info_node(key, value)

      if key == "BQ_NAME"
        self.name = value
        self.save
      end
    end


    collection.xpath("document").each do |doc_xml|
      id = doc_xml.at_xpath("id")
      next if id.nil?

      document = self.find_or_create_document(id.content)
      document.load_from_xml(doc_xml)
    end
  end

  def set_info_node(type, value)
    return if type.nil?
    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      self.info_nodes.create({:key => type, :title => value })
    else
      info_node.title = value
      info_node.save
    end
  end

  def get_info_node(type)
    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      return nil
    else
      return info_node.title
    end
  end 


end
