class Uniprot < ActiveRecord::Base
  attr_accessible :entry_name, :gene_name, :length, :organism, :protein_names, :uniprot_id

  def self.desc(id)
    uniprot = Uniprot.find_by_uniprot_id(id)
    if uniprot.nil?
      return ""
    else
      return uniprot.desc
    end
  end

  def self.fetch(id)
    uniprot = self.find_by_uniprot_id(id) 
    if uniprot.nil?
      begin
        Uniprot.transaction do
          uniprot = Uniprot.new({:uniprot_id => id})

          url = "http://www.uniprot.org/uniprot/#{id}.xml"
          xml = Net::HTTP.get(URI.parse(url))

          doc = Nokogiri::XML(xml) do |config|
            config.noent.strict.noblanks
          end
          doc.remove_namespaces!

          uniprot.entry_name = doc.at_xpath('//entry/name').text unless doc.at_xpath('//entry/name').nil?
          uniprot.gene_name = doc.xpath("//gene/name").map{|i| i.text}.join(" ") 
          uniprot.length = doc.at_xpath('//entry/sequence')['length'] unless doc.at_xpath('//entry/sequence').nil?
          organism1 = doc.xpath('//organism/name')[0].text unless doc.at_xpath('//organism/name').nil?
          organism2 = "(" + doc.xpath('//organism/name')[1].text + ")" unless doc.at_xpath('//organism/name').nil?
          uniprot.organism = [organism1, organism2].join(" ")

          name1 = doc.at_xpath('//protein//fullName').text unless doc.at_xpath('//protein//fullName').nil?
          name2 = "(" + doc.at_xpath('//protein//shortName').text+ ")" unless doc.at_xpath('//protein//shortName').nil?
          uniprot.protein_names = [name1, name2].join(" ")

          logger.debug("#{uniprot.inspect}")
          uniprot.save
          return uniprot
        end
      rescue => e
        logger.debug(e.message)
        logger.debug(e.backtrace)
        return nil
      end 
    else
      return uniprot
    end 
  end

  def priority
    if self.organism.include?("Homo sapiens (Human)")
      return 1
    elsif self.organism.include?("Rattus norvegicus (Rat)")
      return 20
    elsif self.organism.include?("Mus musculus (Mouse)")
      return 10
    else
      return 999
    end
  end
  
  def <=>(another)
    if self.priority < another.priority
      -1
    elsif self.priority > another.priority
      1
    else
      self.protein_names <=> another.protein_names
    end
  end

  def desc
    return "<b>#{entry_name}</b> <span class='organism'>#{organism}</span> #{protein_names} / #{gene_name}"
  end
end
