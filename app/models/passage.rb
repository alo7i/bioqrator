class Passage < ActiveRecord::Base
  attr_accessible :document_id, :offset, :ptype, :ptext, :passage_type
  belongs_to :document
  has_many :annotations, :class_name => "Node", :conditions => "node_type = 'A'"
  has_many :relations, :class_name => "Node", :conditions => "node_type = 'R'"
  
  has_many :nodes
  has_many :info_nodes

  has_many :sentences, :dependent => :destroy

  def self.load_from_xml(document, xml)
    hash = {}
    xml.xpath("infon").each do |infon|
      key = infon["key"]
      value = infon.content
      hash[key] = value
    end

    offset = xml.at_xpath("offset")
    return if offset.nil?
    offset = offset.content.to_i

    text = nil
    text_node = xml.at_xpath("text")
    text = text_node.content unless text_node.nil?

    unless hash["type"].nil?
      passage = document.find_or_create_passage(hash["type"], offset, text, true)
    else
      passage = document.passages.find_by_offset(offset)
      if passage.nil?
        passage = document.passages.create({:offset => offset})
      end

      passage.ptext = text
      passage.save
    end
   
    diff_offset = passage.offset - offset

    if hash["type"] == "title" && !text.blank?
      document.title = text
      document.medline = "BioC" if document.get_info_node("BQ_FROM").nil?
      document.save
    elsif hash["type"] == "abstract" && !text.blank?
      document.abstract = text
      document.medline = "BioC" if document.get_info_node("BQ_FROM").nil?
      document.save
    end

    xml.xpath(".//annotation").each do |a|
      atext = a.at_xpath("text") && a.at_xpath("text").content
      annotation = Node.create_or_find(document, passage, a["id"], "A", atext, nil, nil)
      annotation.load_from_annotation_xml(a, diff_offset)
    end

    xml.xpath(".//relation").each do |r|
      relation = Node.create_or_find(document, passage, r["id"], "R", nil, nil, nil)
      relation.load_from_relation_xml(r)
    end

    xml.xpath("infon").each do |infon|
      key = infon["key"]
      value = infon.content
      passage.set_info_node(key, value)
    end
  end

  def ptype
    self.passage_type
  end

  def set_type(t)
    self.passage_type = t
  end   


  def create_annotation(aid, type, offset, length, atext, representative_name, info_nodes)
    Node.create_annotation(self.document, self, aid, type, offset, length, atext, representative_name, info_nodes)
  end

  def get_offset_text(offset, length)
    s = offset - self.offset
    e = s + length - 1
    ptext[s..e]
  end
  
  def set_info_node(type, value)
    return if type.nil?

    if type == "type "
      self.passage_type = value
      self.save
    end
    
    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      self.info_nodes.create({:key => type, :title => value })
    else
      info_node.title = value
      info_node.save
    end
  end

  def get_info_node(type)
    return nil if type.nil?
    return self.passage_type if type == "type"

    info_node = self.info_nodes.find_by_key(type)
    if info_node.nil?
      return nil
    else
      return info_node.title
    end
  end 
end
