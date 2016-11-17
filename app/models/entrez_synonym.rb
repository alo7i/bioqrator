class EntrezSynonym < ActiveRecord::Base
  attr_accessible :entrez_gene_id, :name

  belongs_to :entrez_gene
end
