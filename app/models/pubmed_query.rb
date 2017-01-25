class PubmedQuery < ActiveRecord::Base
  attr_accessible :count, :term, :date_range
  has_many :pubmed_query_pmids

  def self.esearch(term, date_range)
    return nil if term.nil?

    query = PubmedQuery.where("term=? and date_range=?", term.strip, date_range)

    if query.empty?
      query = PubmedQuery.create({:term => term.strip, :date_range => date_range.strip, :count => 0})
    else
      query = query.first
      if (Time.now - query.updated_at) < 86400 # 86400 = 24 hour * 60 min * 60 sec 
        return query
      else
        PubmedQueryPmid.delete_all(["pubmed_query_id = ?", query.id])
        query.count = 0
        query.updated_at = Time.now
      end
    end

    dates = date_range.split(":")
    url = 'https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=pubmed&email=comeau&retmode=xml&retmax=1000' + 
            '&mindate=' + dates[0] +
            '&maxdate=' + dates[1] +
            '&datetype=pdat' +
            '&term=' + CGI::escape(term)
    logger.debug(url)
    xml = URI.parse(url).read

    logger.debug(xml)
    doc = Nokogiri::XML(xml) do |config|
      config.noent.strict.noblanks
    end

    count_node = doc.at_xpath('//Count') 
    query.count = count_node.content.to_i unless count_node.nil?
    i = 1
    buf = []
    doc.xpath("//IdList/Id").each do |n|
      pmid = n.content.to_i
      next if pmid <= 0

      pie_score = nil
      search = Tire.search "articles" do 
        filter :term, :pmid => pmid
      end
      unless search.results.empty?
        pie_score = search.results.first.ppi_score
      end
      buf << {:pubmed_query_id => query.id, :pmid => pmid, :pubmed_order => i, :pie_score => pie_score}
      i += 1
    end
    query.save
    PubmedQueryPmid.create(buf)
    return query
  end
end
