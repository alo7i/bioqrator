require 'net/http'

class Document < ActiveRecord::Base
  attr_accessible :collection_id, :pmid, :au, :medline, :done, :curatable, 
                  :abstract, :source, :title, :year, :annotations_count, 
                  :relations_count, :ppi_score, :pub_date, :update_date
  belongs_to :collection, :counter_cache => true
  has_many :passages, :dependent => :destroy, :order => :offset
  
  has_many :annotations, :class_name => "Node", :conditions => "node_type = 'A'"
  has_many :relations, :class_name => "Node", :conditions => "node_type = 'R'"
  # has_many :pie_entities, :class_name => "PieEntity", :foreign_key => "pmid", :primary_key => "pmid"
  has_many :nodes, :dependent => :destroy
  has_many :info_nodes, :dependent => :destroy

  validates :pmid, :presence => true
  validates :pmid, :uniqueness => {:scope => :collection_id}

  def remove_annotations
    Document.transaction do 
      Location.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})")
      Reference.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})")
      InfoNode.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})") 
      Node.delete_all("document_id=#{self.id}")
      self.done = false
      self.curatable = true
      self.annotations_count = 0
      self.relations_count = 0
      self.save
    end
  end

  def done?
    self.done
  end

  def curatable?
    self.curatable
  end

  def destroy_bulk(include_infonode = true)
    Location.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})")
    Reference.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})")
    InfoNode.delete_all("node_id in (SELECT id FROM nodes WHERE document_id = #{self.id})") if include_infonode
    Passage.delete_all("document_id=#{self.id}")
    Node.delete_all("document_id=#{self.id}")
  end

  def self.query(str, hash)
    Bio::NCBI.default_email = "dongseop@gmail.com"
    Bio::PubMed.esearch(str, hash)
  end

  def import_pie_annotation(offset_list, word, type)
    aid = create_new_annotation_id
    atext = word
    atype = type || "Protein"
    infos = {
      # "source" => "PIE the search"
    }
    logger.debug("#{aid} #{atext}")
    length = word.size
    offset_list.split(",").each do |offset|
      offset = offset.to_i
      begin
        t = get_offset_text(offset, length)
        t1 = get_offset_text(offset - 1, length)
        t2 = get_offset_text(offset - 2, length)
        t3 = get_offset_text(offset - 3, length)
        t4 = get_offset_text(offset + 1, length)
        t5 = get_offset_text(offset + 2, length)
        t6 = get_offset_text(offset + 3, length)
        t7 = get_offset_text(offset + 4, length)
        t8 = get_offset_text(offset + 5, length)
        t9 = get_offset_text(offset + 6, length)
        t10 = get_offset_text(offset + 7, length)

        if (!t.nil? && t.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset, length)
        elsif (!t1.nil? && t1.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset - 1, length)
        elsif (!t4.nil? && t4.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 1, length)
        elsif (!t2.nil? && t2.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset - 2, length)
        elsif (!t5.nil? && t5.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 2, length)
        elsif (!t3.nil? && t3.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset - 3, length)
        elsif (!t6.nil? && t6.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 3, length)
        elsif (!t7.nil? && t7.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 4, length)
        elsif (!t8.nil? && t8.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 5, length)
        elsif (!t9.nil? && t9.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 6, length)
        elsif (!t10.nil? && t10.casecmp(atext) == 0)
          create_annotation(aid, atext, nil, atype, infos, offset + 7, length)
        end

      rescue => e
        logger.debug("TEST '#{atext}'(#{offset}) with '#{t}'")
        logger.debug("TEST #{atext}(#{offset}) with #{t}, #{t1}, #{t2}, #{t3}, #{t4}, #{t5}, #{t6}")
        logger.debug("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!! #{e.message}")
      end
    end
  end

  def import_pie_entities
    self.pie_entities.each do |e|
      aid = create_new_annotation_id
      atext = e.name
      infos = {
        "source" => "PIE the search"
      }
      logger.debug("#{aid} #{atext}")
      e.pie_entity_locations.each do |l|
        begin
          t = get_offset_text(l.offset, l.length)
          t1 = get_offset_text(l.offset - 1, l.length)
          t2 = get_offset_text(l.offset - 2, l.length)
          t3 = get_offset_text(l.offset - 3, l.length)
          t4 = get_offset_text(l.offset + 1, l.length)
          t5 = get_offset_text(l.offset + 2, l.length)
          t6 = get_offset_text(l.offset + 3, l.length)

          if (!t.nil? && t == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset, l.length)
          elsif (!t1.nil? && t1 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset - 1, l.length)
          elsif (!t2.nil? && t2 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset - 2, l.length)
          elsif (!t3.nil? && t3 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset - 3, l.length)
          elsif (!t4.nil? && t4 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset + 1, l.length)
          elsif (!t5.nil? && t5 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset + 2, l.length)
          elsif (!t6.nil? && t6 == atext)
            create_annotation(aid, atext, nil, "Protein", infos, l.offset + 3, l.length)
          end

        rescue => e
          logger.debug("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!! #{e.message}")
        end
      end
    end
  end
  
  def get_offset_text(offset, length)
    passage = get_passage_from_offset(offset)
    if passage.nil?
      return nil
    else
      passage.get_offset_text(offset, length)
    end
  end

  def annotation_overlap?(offset, length)
    self.annotations.each do |a|
      a.locations.each do |l|
        if l.overlap?(offset, length)
          logger.debug("#{offset}:#{length} overlapped with #{l.offset}:#{l.length} (#{a.node_text})")
          return true 
        end
      end
    end
    return false
  end

  def authors
    self.au.gsub(/\n/,', ') unless self.au.nil?
  end 

  def fetch(update_date = nil, override = false)
    doc = DocumentCache.fetch(self.pmid, self.ppi_score, update_date)
    if doc.nil?
      # cannot retrieve from PubMed
      self.title = ""
      self.abstract = ""
      self.au = ""
      self.year = nil
      self.source = nil
      self.medline = "N/A"
      self.ppi_score = nil
      self.pub_date = nil
    else
      self.title = doc.title.gsub("\u00a0", ' ')
      self.abstract = doc.abstract.gsub("\u00a0", ' ')
      self.au = doc.au
      self.year = doc.year
      self.source = doc.source
      self.medline = "PubMed"
      self.ppi_score = doc.ppi_score unless doc.ppi_score.nil?
      self.pub_date = doc.pubdate
      self.update_date = doc.update_date
    end
    self.save

    self.find_or_create_passage('title', 0, self.title + " ", override)

    unless self.abstract.nil?
      self.find_or_create_passage('abstract', self.title.size + 1, self.abstract + " ", override)
    end
  end

  def get_passage_from_offset(offset)
    if passages.nil? || passages.empty?
      return nil
    end 

    ret = passages.first
    passages.each do |p|
      if p.offset > offset && ret.offset <= offset
        return ret
      end
      ret = p
    end
    return ret    
  end

  def find_or_create_passage(ptype, offset, ptext, override = true)
    logger.debug("CREATE PASSAGE? #{ptype} #{offset} #{override} ==============================")
    if ptype.nil?
      raise 'passage_type is null'
    end

    offset = offset.to_i
    
    if offset < 0 
      raise 'offset cannot be negative'
    end

    passage = nil

    abstract_offset = self.title.size + 1

    if ptype == "abstract" && offset < abstract_offset
      offset = abstract_offset
    end
    
    self.passages.each do |p|
      if p.passage_type == ptype
        passage = p
      end

    end

    if passage.nil?
      passage = self.passages.new
      override = true
    end

    if override
      passage.ptext = ptext unless ptext.nil?
      passage.offset = offset
    end
    passage.passage_type = ptype
    passage.save

    return passage
  end 

  def create_annotation(aid, atext, representative_name, type, info_nodes, offset, length)
    atext2 = get_offset_text(offset, length)
    passage = get_passage_from_offset(offset)
    raise "Not valid offset" if passage.nil?

    collection.find_or_create_entity_type(type)
    return passage.create_annotation(aid, type, offset, length, atext2, representative_name, info_nodes)
  end

  def create_relation(rid, type, refs, info_nodes)
    Node.create_relation(self, rid, type, refs, info_nodes)
  end

  def recommend_passage(refs)
    refs.each do |item|
      a = self.annotations.find_by_aid(item.ref_id).first
      return a.passage unless a.nil?
    end
    refs.each do |item|
      r = self.relations.find_by_aid(item.ref_id).first
      return r.passage unless r.nil?
    end

    return self.passages.first unless self.passages.empty?
    return nil
  end

  def load_from_xml(xml)
    self.done = false
    self.curatable = true
    self.save
    xml.xpath("infon").each do |infon|
      key = infon["key"]
      value = infon.content
      set_info_node(key, value)

      if key == "BQ_PPISCORE" || key == "BQ_PIESCORE"
        self.ppi_score = value.to_f
        self.save
      end

      if key == "BQ_DONE" 
        self.done = (value == "YES")
        self.save
      end

      if key == "BQ_CURATABLE" 
        self.curatable = (value == "YES")
        self.save
      end

      if key == "BQ_FROM"
        self.medline = value
        self.save
      end
    end

    xml.xpath("relation").each do |r|
      relation = Node.create_or_find(self, nil, r["id"], "R", nil, nil, nil)
      relation.load_from_relation_xml(r)
    end

    xml.xpath("passage").each do |p|
      Passage.load_from_xml(self, p)
    end

    self.adjust_relation_child
  end

  

  def set_info_node(type, value)
    return if type.nil?
    
    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      self.info_nodes.create({:key => type, :title => value })
    else
      info_node.title = value
      info_node.save
    end
  end

  def get_info_node(type)
    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      return nil
    else
      return info_node.title
    end
  end 

  def create_new_annotation_id
    max = 0
    Node.where("document_id = ?", self.id).each do |a|
      next if a.node_id[/^A/i].nil?
      num = a.node_id.scan(/\d*/).join("").to_i
      if (num > max) 
        max = num
      end
    end
    return "A#{max + 1}"
  end 

  def create_new_relation_id
    max = 0
    Node.where("document_id = ?", self.id).each do |a|
      next if a.node_id[/^R/i].nil?
      num = a.node_id.scan(/\d*/).join("").to_i
      if (num > max) 
        max = num
      end
    end
    return "R#{max + 1}"
  end 

  def find_node_id(node_ref_id)
    node = self.nodes.where("node_id = ?", node_ref_id).first
    if node.nil?
      return nil
    else
      return node.id
    end
  end

  def adjust_relation_child
    self.relations.each do |r|
      r.references.each do |p|
        logger.debug("P #{p.inspect}")
        if p.child.nil? && !p.ref_id.nil?
          p.child_id = self.find_node_id(p.ref_id)
          p.save
        end
      end
    end
  end
end
