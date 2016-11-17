class Location < ActiveRecord::Base
  attr_accessible :node_id, :length, :offset
  belongs_to :node, :counter_cache => true

  def overlap?(o, l)
    if (offset >= o + l) || (o >= offset + length)
      return false
    else 
      return true
    end
  end

  def has_overlap?(ranges) 
    ranges.each do |r|
      if self.overlap?(r[0], r[1])
        logger.debug("#{self.offset}:#{self.length} ? #{ranges.inspect}")
        return true
      end
    end
    return false
  end
end
