class Node < ActiveRecord::Base
  attr_accessible :document_id, :node_id, :node_text, :node_type, :passage_id, :entity_type, :representative_name

  belongs_to :document
  belongs_to :passage

  has_many :info_nodes, :dependent => :destroy
  has_many :references, :dependent => :destroy, :order => "order_no"
  has_many :locations, :dependent => :destroy, :order => "offset, length"
  has_many :parents, :class_name => "Reference", :foreign_key => "child_id"

  before_destroy :check_parents

  def check_parents
    ref_nodes = []
    self.document.nodes.each do |n|
      n.references.each do |r|
        if r.child_id == self.id
          ref_nodes << "'#{n.node_id}'"
        end
      end
    end 
    unless ref_nodes.empty? 
      errors.add(:base, "There are references in this node. (#{ref_nodes.join(',')})")
      return false
    end
  end

  after_create do |node|
    if node.node_type == "R"
      node.document.increment(:relations_count)
    elsif node.node_type == "A"
      node.document.increment(:annotations_count)
    end
  end

  after_destroy do |node|
    if node.node_type == "R"
      node.document.decrement(:relations_count)
    elsif node.node_type == "A"
      node.document.decrement(:annotations_count)
    end
  end

  def annotation?
    return self.node_type == "A"
  end

  def relation?
    return self.node_type == "R"
  end

  def to_csv
    if self.annotation?
      [self.document.pmid, self.node_type_str, self.node_id, self.entity_type, self.node_text, self.locstr, self.infonstr].to_csv 
    else
      [self.document.pmid, self.node_type_str, self.node_id, self.entity_type, nil, self.refstr, self.infonstr].to_csv 
    end
  end

  def node_type_str
    if self.annotation?
      "annotation"
    elsif relation?
      "relation"
    end
  end

  def load_from_relation_xml(xml)
    self.load_from_xml(xml)

    unless self.references.empty?
      self.references.destroy_all
    end

    idx = 0
    xml.xpath("node").each do |node|
      unless node["refid"].nil?
        self.references.create({:ref_id => node["refid"], :role => node["role"], :order_no => idx })
        idx += 1
      end
    end
  end
  
  def dbid?
    self.info_nodes.each do |i|
      if i.key == "UniProtKB" || i.key == "EntrezGene"
        return true
      end
    end
    return false
  end

  def get_infos
    ret = []
    self.info_nodes.each do |i|
      desc = nil
      link = nil
      if i.key == 'UniProtKB'
        desc = Uniprot.desc(i.title) 
        link = "http://www.uniprot.org/uniprot/#{i.title}"
      elsif i.key == 'EntrezGene' 
        desc = EntrezGene.desc(i.title) 
        link = "http://www.ncbi.nlm.nih.gov/gene/#{i.title}"
      end
      ret << {:key => i.key, :value => i.title, :desc => desc, :link => link}
    end
    return ret
  end

  def desc
    uniprot_id = get_info_node("UniProtKB")
    entrez_id = get_info_node("EntrezGene")
    ret = []
    unless uniprot_id.nil?
      ret << "[#{uniprot_id}] " + Uniprot.desc(uniprot_id)
    end
    unless entrez_id.nil?
      ret << "[#{entrez_id}] " + EntrezGene.desc(entrez_id)
    end

    return ret.join(", ")
  end

  def load_from_xml(xml)
    xml.xpath("infon").each do |infon|
      key = infon["key"]
      value = infon.content

      set_info_node(key, value)
    end
  end

  def load_from_annotation_xml(xml, diff_offset)
    self.load_from_xml(xml)

    unless self.locations.empty?
      self.locations.destroy_all
    end

    xml.xpath("location").each do |l|
      self.locations.create({:offset => (l["offset"].to_i + diff_offset), :length => l[:length]})
    end
  end

  def add_location(offset)
    length = node_text.size
    if self.document.annotation_overlap?(offset, length)
      self.errors.add(:base, "Location overlapped")
      return false
    end
    return self.locations.create({:offset => offset, :length => length})
  end

  def add_all
    text = self.node_text
    cnt = 0
    logger.debug(text)
    logger.debug(text.inspect)
    logger.debug("IN DSADASD #{self.document.passages.size}")
    self.document.passages.each do |p|
      offset = 0
      logger.debug("IN PASSAGE #{offset.nil?}")
      while !offset.nil?
        pattern = /\b(#{Regexp.escape(self.node_text)})\b/i
        offset = p.ptext.index(pattern, offset)
        logger.debug("==> #{pattern.inspect} result #{offset}")
        unless offset.nil?
          unless self.document.annotation_overlap?(offset + p.offset, text.length)
            if self.add_location(offset + p.offset)
              cnt += 1
            end
          end
          offset = offset + 1
        end
      end
    end
    return cnt
  end

  def remove_location(offset)
    self.locations.each do |l|
      if l.offset == offset
        l.destroy
        return true
      end
    end
    self.errors.add(:base, "cannot find offset")
    return false
  end
  def locstr
    locations.map{|l| "#{l.offset}:#{l.length}"}.join(";")
  end

  def refstr
    references.map do |r| 
      if r.role.blank?
        r.ref_id
      else
        [r.ref_id, r.role].join(":")
      end
    end.join(";")
  end  

  def infonstr
    return nil if info_nodes.empty?
    info_nodes.map{|r| "#{r.key}:#{r.title}"}.join(";")
  end

  def locstr_with_br(ranges)
    ret = []
    locations.each do |l|
      if l.has_overlap?(ranges)
        ret << "<span style='text-decoration:line-through;color:red' title='overlapped location cannot be displayed!'>#{l.offset}:#{l.length}</span>"
      else
        ret << "#{l.offset}:#{l.length}"
        ranges << [l.offset, l.length]
      end
    end
    ret.join(", ").html_safe
  end


  def self.create_or_find(document, passage, node_id, node_type, node_text, representative_name, info_nodes)
    logger.debug("IN create_or_find '#{node_text}'")
    if node_id && !node_id.blank?
      node = document.nodes.find_by_node_id(node_id)
    end
    
    if node.nil?
      node = document.nodes.new({:node_id => node_id})
      node.passage = passage unless passage.nil?
    end

    node.node_type = node_type unless node_type.nil?
    node.node_text = node_text unless node_text.nil?
    node.representative_name = representative_name
    node.save

    info_nodes.each do |key, value|
      node.set_info_node(key, value)
    end unless info_nodes.nil?

    return node
  end

  def self.create_annotation(document, passage, node_id, type, offset, length, node_text, representative_name, info_nodes)
    logger.debug("IN create_annotation '#{node_text}'")

    node = document.nodes.find_by_node_id(node_id)
    if !node_id.blank? && !node.nil?
      if node_text.casecmp(node.node_text) != 0
        raise 'Same node id exist'
      end
    end
    if document.annotation_overlap?(offset, length)
      raise 'Entity region is overlapped'
    end

    node = Node.create_or_find(document, passage, node_id, "A", node_text, representative_name, info_nodes)
    node.entity_type = type
    node.save

    node.locations.create({:offset => offset, :length => length})

    return node
  end

  def self.create_relation(document, node_id, type, refs, info_nodes)
    node = document.nodes.find_by_node_id(node_id)
    if !node_id.blank? && !node.nil?
      raise 'Same node id exist'
    end

    node = Node.create_or_find(document, nil, node_id, "R", nil, nil, info_nodes)
    node.entity_type = type
    node.save

    node.update_references(refs)
  end

  def position
    if locations.empty?
      0
    else
      locations.first.offset
    end
  end

  def type_str
    if self.entity_type.nil?
      return "-"
    else
      return self.entity_type
    end
  end

  def set_info_node(type, value)
    return if type.nil?
    
    if type == "type"
      EntityType.find_or_create(self.document.collection, value)
      self.entity_type = value
      self.save
      return
    elsif type == "representative_name"
      self.representative_name = value
      self.save
      return
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

    if type == "type"
      return self.entity_type
    elsif type == "representative_name"
      return self.representative_name
    end

    # info_node = self.info_nodes.find_by_key(type)
    # if info_node.nil?
    #   return nil
    # else
    #   return info_node.title
    # end

    self.info_nodes.each do |n|
      if n.key == type
        return n.title
      end
    end
    return nil
  end 

  def remove_info_node_except(keyset)
    self.info_nodes.each do |info_node|
      if !keyset.include?(info_node.key)
        info_node.destroy
      end
    end
  end

  def find_or_create_reference(ref_id, role, order)
    ref = self.references.find_by_order_no(order)
    if ref.nil?
      ref = self.references.new()
    end

    ref.ref_id = ref_id
    ref.role = role
    ref.order_no = order
    ref.child_id = self.document.find_node_id(ref_id)
    ref.save
  end

  def update_references(refs)
    self.references.each do |r|
      r.destroy if r.order_no >= refs.size
    end

    refs.each_with_index do |ref, idx|
      self.find_or_create_reference(ref[:ref_id], ref[:role], idx)
    end
  end
end



