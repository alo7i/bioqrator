require "open-uri"
class SaveQueryCache < ActiveRecord::Base
  attr_accessible :cache_date, :duration, :save_query_docs_count, :save_query_id, :term
  belongs_to :save_query
  has_many :save_query_docs, :dependent => :destroy

  def valid_cache?
    return self.duration != nil && self.cache_date != nil
  end

  def self.fetch(term, date)
    sqc = nil
    SaveQueryCache.transaction do
      sqc = SaveQueryCache.where("term = ? and duration = ?", term, date).first
      if sqc.nil?
        sqc = SaveQueryCache.create({:term => term, :duration => date})
      end

      sqc.fetch
    end
    return sqc
  end
  
  def fetch
    self.term = self.save_query.term unless self.save_query.nil?
    url = 'https://www.ncbi.nlm.nih.gov/CBBresearch/Wilbur/IRET/PIE/getpie.cgi?' + 
          'term=' + CGI::escape(self.term) +
          '&date=' + self.duration
    # logger.debug(url)
    result = URI.parse(url).read
    # logger.debug(result)
    num_rows = 0
    self.save_query_docs.destroy_all
    result.split("\n").each do |line|
      # logger.debug(line)
      line = line.strip
      data = line.split("\t")
      if data.size >= 3
        num_rows += 1
        if num_rows <= 1000 
          self.save_query_docs.create({:pmid => data[0], :score => data[1], :pubdate => data[2], :update_date => data[3]})
        end
      end
    end
    self.cache_date = Time.now
    self.save_query_docs_count = num_rows
    self.save
  end

  def self.generate_intervals(duration, startDate, offset, pageSize)
    logger.debug("Duration:#{duration} start #{startDate} offset #{offset} pageSize #{pageSize}")
    intervals = []
    case duration
    when "Weekly"
      day = startDate - offset.week
      if day > Date.today
        day = Date.today
      end

      (0...pageSize).each do |i|
        intervals << "#{day.beginning_of_week.strftime('%Y/%m/%d')}:#{day.end_of_week.strftime('%Y/%m/%d')}"
        day = day - 1.week
      end
    when "Monthly"
      day = startDate - offset.month
      if day > Date.today
        day = Date.today
      end
      logger.debug("day#{day}!!!  Duration:#{duration} start #{startDate} offset #{offset} pageSize #{pageSize}")

      (0...pageSize).each do |i|
        intervals << "#{day.beginning_of_month.strftime('%Y/%m/%d')}:#{day.end_of_month.strftime('%Y/%m/%d')}"
        day = day - 1.month
      end
    when "Quarterly"
      day = startDate - (offset * 3).month
      if day > Date.today
        day = Date.today
      end

      (0...pageSize).each do |i|
        intervals << "#{day.beginning_of_quarter.strftime('%Y/%m/%d')}:#{day.end_of_quarter.strftime('%Y/%m/%d')}"
        day = day - 3.month
      end
    when "Yearly"
      day = startDate - offset.year
      if day > Date.today
        day = Date.today
      end

      (0...pageSize).each do |i|
        intervals << "#{day.beginning_of_year.strftime('%Y/%m/%d')}:#{day.end_of_year.strftime('%Y/%m/%d')}"
        day = day - 1.year
      end
    end
    logger.debug(intervals.inspect)
    return intervals
  end

  def self.generate_intervals_for_pages(duration, page, pageSize)
    return self.generate_intervals(duration, Date.today, (page - 1) * pageSize, pageSize)
  end

  def self.generate_intervals_for_date(duration, date, pageSize)
    return self.generate_intervals(duration, date, -5, pageSize)
  end
end
