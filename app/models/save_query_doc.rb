class SaveQueryDoc < ActiveRecord::Base
  attr_accessible :pmid, :save_query_cache_id, :score, :pubdate, :update_date
  belongs_to :save_query_cache

  def self.get_update_date(pmid)
    pmid = pmid.to_i
    ret = SaveQueryDoc.find_by_sql("SELECT MAX(update_date) AS update_date FROM save_query_docs WHERE pmid = '#{pmid}'");
    if ret.empty? 
      return nil
    else
      return ret.first["update_date"]
    end
  end

  def pie_score
    score
  end
end
