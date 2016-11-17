class EntityType < ActiveRecord::Base
  attr_accessible :category, :collection_id, :name, :nomenclature, :url, :for_entity, :for_relation
  # belongs_to :collection, :counter_cache => true
  belongs_to :collection
  validates :name, :presence => true
  validates :name, :uniqueness => {:scope => :collection_id}

  before_destroy :check_nodes
  after_update :update_nodes

  has_many :nodes
  has_many :type_properties
    
  CATEGORY = {
    "EntrezGene" => { 
      :description => "Gene with Entrez ID",
      :nomenclature =>"Gene", 
      :query =>"http://www.ncbi.nlm.nih.gov/gene/?term=",
      :url =>"http://www.ncbi.nlm.nih.gov/gene/"
    }, 
    "UniProtKB" => { 
      :description =>"Protein with UniProtKB ID",
      :nomenclature =>"Protein", 
      :query =>"http://www.uniprot.org/uniprot/?sort=score&query=",
      :url =>"http://www.uniprot.org/uniprot/"
    },
    "PPI" => { 
      :description =>"Protein-Protein Interaction",
      :nomenclature =>"Protein", 
      :query =>"http://www.uniprot.org/uniprot/?sort=score&query=",
      :url =>""
    }
 
  }

  RECOMMENDER = {
    "entrez" => "EntrezGene",
    "uniprot" => "UniProtKB"
  }

  def check_nodes
    ret = Node.find_by_sql(["SELECT * FROM nodes join documents on nodes.document_id = documents.id WHERE documents.collection_id = ? and entity_type = ?", self.collection_id, self.name])
    unless ret.empty?
      errors.add(:base, "There are references for this type")
      return false
    end
    return true
  end

  def update_nodes
    logger.debug(changed_attributes.inspect)
    collection.documents.each do |d|
      Node.update_all({:entity_type => self.name}, ["document_id = ? and entity_type=?", d.id, changed_attributes["name"]])
    end       
  end

  def self.options_for_category
    [["-",""]] + CATEGORY.map{|k, v| [v[:description], k]} 
  end

  def self.find_or_create(collection, name)
    entity_type = collection.entity_types.find_by_name(name)

    if entity_type.blank?
      collection.entity_types.create({:name => name})
    end
  end
end
