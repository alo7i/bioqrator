class EntrezGene < ActiveRecord::Base
  attr_accessible :description, :name, :species, :priority
  has_many :entrez_synonyms

  def self.species_map(name)    
    ["Homo sapiens", "Mus musculus", "Rattus norvegicus"]
  end

  def self.desc(id)
    gene = EntrezGene.find_by_id(id)
    if gene.nil?
      return ""
    else
      return gene.desc
    end
  end

  def desc
    "<b>#{name}</b> <span class='organism'>#{species}</span> #{description}"
  end
end
