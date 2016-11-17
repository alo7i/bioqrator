class PieEntity < ActiveRecord::Base
  attr_accessible :entrez_id, :name, :pmid

  has_many :pie_entity_locations
end
