class PubmedQueryPmid < ActiveRecord::Base
  attr_accessible :pie_score, :pmid, :pubmed_query_id, :pubmed_order
  belongs_to :pubmed_query
end
