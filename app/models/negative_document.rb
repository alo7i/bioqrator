class NegativeDocument < ActiveRecord::Base
  attr_accessible :collection_id, :pmid
  validates :pmid, :uniqueness => {:scope => :collection_id}
  belongs_to :collection
end
