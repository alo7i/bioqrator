class UniprotQuery < ActiveRecord::Base
  attr_accessible :order_no, :term, :uniprot_id

  belongs_to :uniprot, :primary_key => "uniprot_id", :foreign_key => "uniprot_id"

  def self.query(term)
    result = UniprotQuery.where("term = ?", term).includes(:uniprot).order("order_no")
    if result.empty?
      ret_array = []
      url = "http://www.uniprot.org/uniprot/?query=#{CGI.escape(term)}&sort=score&limit=30&format=tab&columns=id,entry%20name,protein%20names,genes,organism,length"
      logger.debug(url)
      ret = Net::HTTP.get(URI.parse(url))
      i = -1
      lines = ret.split("\n")
      if lines.size >= 2
        lines.each do |line|
          i += 1
          next if i <= 0
          item = line.split("\t")
          uniprot = Uniprot.find_by_uniprot_id(item[0])
          if uniprot.nil?
            uniprot = Uniprot.create({:uniprot_id => item[0], :entry_name => item[1], 
                      :protein_names => item[2], :gene_name => item[3], :organism => item[4], :length => item[5]})
          end
          ret_array << uniprot
          UniprotQuery.create({:order_no => i, :term => term, :uniprot_id => uniprot.uniprot_id})
        end
        return ret_array
      else
        UniprotQuery.create({:order_no => 0, :term => term, :uniprot_id => nil})
        return []
      end
    else
      if result.first.order_no == 0
        return []
      else
        return result.map {|uq| uq.uniprot} 
      end
    end
  end
end
