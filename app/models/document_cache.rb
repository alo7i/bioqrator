require 'htmlentities'
require "open-uri"

class DocumentCache < ActiveRecord::Base
  attr_accessible :abstract, :authors, :id, :source, :title, :year, :medline, :pmid, :ppi_score, :pubdate, :update_date
  
  def fetch
    url = 'https://eutils.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?db=pubmed&email=comeau&retmode=xml&id=' +
      self.pmid.to_s
    art = nil

    (1..10).each do |i|
      xml = URI.parse(url).read
      self.medline = xml
      logger.debug(xml)
      doc = Nokogiri::XML(xml) do |config|
        config.noent.strict.noblanks.nonet
      end
      art = doc.at_xpath('//PubmedArticle') || doc.at_xpath('//BookDocument')
      
      break if !art.nil?
      sleep(1)
      logger.error("#{i}th TRY!: #{url}\n#{xml}") 
    end

    load_from_xml(art) unless art.nil?

    return !art.nil?
  end
  
  def load_from_xml(art)
    logger.debug(art)
    self.pmid = art.at_xpath('descendant::PMID').text
    
    title_node = art.at_xpath('//ArticleTitle') || art.at_xpath('//BookTitle') 
    self.title = title_node.text

    abstract = art.at_xpath('//Abstract')
    if abstract
      self.abstract = ''
      abstract.xpath('child::AbstractText').each do |at|
        label = at.attr('Label')
        self.abstract += ' ' if ! self.abstract.empty? # space between sections
        self.abstract += ( label + ': ' ) if label
        self.abstract += at.text
      end
    end

    self.au = art.xpath('descendant::AuthorList/Author').map{|a| [a.at_xpath("LastName"), a.at_xpath("Initials")].join(" ")}.join(", ")


    yy =  art.at_xpath(".//PubMedPubDate[PubStatus='pubmed']/Year") || art.at_xpath(".//PubDate/Year") || art.at_xpath(".//PubDate/MedlineDate")
    if yy.nil?

    else
      self.year = yy.content.to_i
    end
    # R I Med J (2013). 2013 Feb 1;96(2):19-21.
    journal = art.at_xpath(".//ISOAbbreviation")
    if journal.nil?
      self.source = art.at_xpath(".//BookTitle").text + "," + art.at_xpath(".//PublisherName").text 
    else 
      so_array = [art.at_xpath(".//ISOAbbreviation").text]

      yy = art.at_xpath(".//PubMedPubDate[PubStatus='pubmed']/Year") || art.at_xpath(".//PubDate/Year") || art.at_xpath(".//ArticleDate/Year") || art.at_xpath(".//PubDate/MedlineDate")
      mm = art.at_xpath(".//PubMedPubDate[PubStatus='pubmed']/Month") || art.at_xpath(".//PubDate/Month") || art.at_xpath(".//ArticleDate/Month")
      dd = art.at_xpath(".//PubMedPubDate[PubStatus='pubmed']/Day") || art.at_xpath(".//PubDate/Day") || art.at_xpath(".//ArticleDate/Day")

      logger.debug("YYMMDD#{yy}:#{mm}:#{dd}")
      if dd.nil? && !mm.nil? && !yy.nil?
        pubdate = Time.new(yy.text, mm.text, 1)
        so_array << pubdate.strftime("%Y %b") unless pubdate.nil?
      elsif mm.nil? && !yy.nil?
        pubdate = Time.new(yy.text, 1, 1)
        so_array << pubdate.strftime("%Y") unless pubdate.nil?
      elsif !dd.nil? && !mm.nil? && !yy.nil?
        pubdate = Time.new(yy.text, mm.text, dd.text)
        so_array << pubdate.strftime("%Y %b %-d") unless pubdate.nil?
      end
      
      vol = art.at_xpath(".//JournalIssue/Volume")
      issue = art.at_xpath(".//JournalIssue/Issue")

      so_array << "#{vol.text}(#{issue.text})" unless vol.nil? || issue.nil?
      art.xpath(".//ELocationID").each do |eloc|
        so_array << eloc.attr('EIdType') + ": " + eloc.text
      end
      self.source = so_array.join(". ")

      page = art.at_xpath(".//Pagination/MedlinePgn")

      self.source = self.source + ":#{page.text}" if !page.nil? && !page.text.blank?
    end
    sqd = SaveQueryDoc.where("pmid = ?", self.pmid).last
    unless sqd.nil?
      self.ppi_score = sqd.score
      self.pubdate = sqd.pubdate
    end

    yy = art.at_xpath(".//DateRevised/Year") || art.at_xpath(".//DateCompleted/Year") || art.at_xpath(".//DateCreated/Year") || "2000"
    mm = art.at_xpath(".//DateRevised/Month") || art.at_xpath(".//DateCompleted/Month") || art.at_xpath(".//DateCreated/Month") || "01"
    dd = art.at_xpath(".//DateRevised/Day") || art.at_xpath(".//DateCompleted/Day") || art.at_xpath(".//DateCreated/Day") || "01"

    begin
      self.update_date = Date.strptime("#{yy.text}-#{mm.text}-#{dd.text}", "%Y-%m-%d")
    rescue ArgumentError
      begin
        self.update_date = Date.strptime("#{yy.text}-#{mm.text}-#{dd.text}", "%Y-%b-%d")
      rescue ArgumentError
      end
    end
    coder = HTMLEntities.new
    self.title = coder.decode(self.title)
    self.abstract = coder.decode(self.abstract)
    self.save
  end

  def self.fetch(pmid, score = nil, update_date = nil)
    doc = DocumentCache.find_by_id(pmid)
    if doc.nil?
      doc = DocumentCache.new({:id => pmid, :pmid => pmid, :ppi_score => score})
      if !doc.fetch
        return nil
      end
    else
      update_date = SaveQueryDoc.get_update_date(pmid) if update_date.nil?
      update_date = Date.strptime(update_date) if update_date.kind_of? String
      logger.debug(doc.update_date.inspect)
      logger.debug(update_date.inspect)
      if !update_date.nil? && doc.update_date < update_date
        doc.fetch
      end
    end
    doc.ppi_score = score unless score.nil?
    doc.save
    return doc
  end

  def self.expire(pmid, pubdate, update_date) 
    doc = DocumentCache.find_by_id(pmid)

    unless doc.nil?
      if (!pubdate.nil? && (doc.pubdate.nil? || doc.pubdate < pubdate)) ||
         (!update_date.nil? && (doc.update_date.nil? || doc.update_date < update_date)) then
        doc.destroy
      end
    end
  end
end
