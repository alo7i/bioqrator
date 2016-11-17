class Recommender < ActiveRecord::Base
  attr_accessible :collection_id, :desc, :link, :name, :type_str
  belongs_to :collection

  validates :name, :presence => true
  validates :name, :uniqueness => {:scope => :collection_id}
  validates :link, :uniqueness => {:scope => :collection_id}

  validates :link, :presence => true

end
