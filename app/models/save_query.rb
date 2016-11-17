class SaveQuery < ActiveRecord::Base
  attr_accessible :collection_id, :duration, :term
  belongs_to :collection
  has_many :save_query_caches, :class_name => "SaveQueryCache", :dependent => :destroy

  validates :term, :presence => true
  validates :duration, :presence => true
  validates :term, :uniqueness => {:scope => :collection_id}

  def find_or_create_cache(date)
    cache = self.save_query_caches.where("duration = ?", date).first
    if cache.nil? 
      cache = self.save_query_caches.create({:duration => date})
    end
    return cache
  end

  def destroy_bulk
    SaveQueryCache.delete_all("save_query_id = #{self.id}")
    SaveQueryDoc.delete_all("save_query_cache_id IN (SELECT id FROM save_query_caches WHERE save_query_id = #{self.id})")
  end
  
  def get_caches_in_intervals(intervals)
    rets = self.save_query_caches.where("duration <= ? AND duration >= ?", intervals.first, intervals.last)
    caches = {}
    rets.each do |r|
      caches[r.duration] = r
    end
    return caches
  end

  def query(date, force_reload)
    cache = self.find_or_create_cache(date)

    if !cache.valid_cache? || force_reload
      SaveQuery.transaction do
        cache.fetch
      end
    end
    return cache
  end
end
