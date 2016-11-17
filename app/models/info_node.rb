class InfoNode < ActiveRecord::Base
  attr_accessible :collection_id, :document_id, :key, :node_id, :node_type, :passage_id, :title

  belongs_to :collection
  belongs_to :document
  belongs_to :node
  belongs_to :passage

  def self.duplicate_key?(arr)
    l = arr.size
    logger.debug(arr.inspect)
    (0...l).each do |i|
            logger.debug(i)

      next if arr[i].nil?
      key1 = arr[i].strip
      next if key1.blank?
      (i+1...l).each do |j|
            logger.debug(j)
        next if arr[j].nil?
        key2 = arr[j].strip
        if key1 == key2
          logger.debug("#{key1} == #{key2}")
          return true
        end
      end
    end
    return false
  end
end
