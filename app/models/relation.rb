class Relation < ActiveRecord::Base
  # attr_accessible :passage_id, :sentence_id, :rid, :document_id, :entity_type_id, :entity_key
  # belongs_to :document, :counter_cache => true
  # belongs_to :sentence
  # belongs_to :passage
  # has_many :participants, :order => :order_no, :foreign_key => "participates_in", :dependent => :destroy
  # belongs_to :entity_type

  # validates :rid, :presence => true
  # before_save :check_unique_rid

  # def check_unique_rid
  #   self.document.relations.each do |r|
  #     if r.rid == self.rid && ( self.id != nil && r.id != self.id)
  #       errors[:rid] = "Same relation id exists"
  #       return false
  #     end
  #   end
  #   self.document.annotations.each do |a|
  #     if self.rid == a.aid
  #       errors[:rid] = "Same annotation id exists"
  #     end
  #   end   
  # end

  # def desc
  #   "Relation"  
  # end

  # def type_str 
  #   self.entity_type.name unless self.entity_type.nil?
  # end

  # def self.create_or_find(document, passage, type_id, id, key = nil)
  #   if id.nil?
  #     raise 'Relation id is null'
  #   end

  #   annotation = document.annotations.find_by_aid(id)
  #   if !annotation.nil? 
  #     # annotation with negative offset is a fake annotation. 
  #     # Handle this at the end of this method
  #     raise "Cannot make a relation bacause there is an annotation with #{id}"
  #   end

  #   relation = document.relations.find_by_rid(id)
  #   if relation.nil?
  #     relation = document.relations.new({:rid => id})
  #     relation.passage = passage unless passage.nil?
  #   end

  #   relation.entity_type_id = type_id
  #   relation.entity_key = key unless key.nil?
  #   relation.save

  #   return relation
  # end


  # def update_participants(refs)
  #   self.participants.each do |p|
  #     p.destroy if p.order_no >= refs.size
  #   end
  #   refs.each_with_index do |ref, idx|
  #     self.find_or_create_participant(ref[:label], ref[:ref_id], idx)
  #   end
  # end

  # def find_or_create_participant(label, ref_id, order)
  # 	annotation = document.annotations.find_by_aid(ref_id)
  # 	relation = document.relations.find_by_rid(ref_id)

  # 	if annotation.nil? and relation.nil?
  #     annotation = document.annotations.create({aid: ref_id})
  # 	elsif !annotation.nil? and !relation.nil?
  # 		raise 'there are the same id in annotations and relations'
  # 	end

  # 	participant = self.participants.find_by_order_no(order)
  # 	if participant.nil?
  # 		participant = self.participants.new
  #   else
  #     participant.relation_id = nil
  #     participant.annotation_id = nil
  # 	end

  # 	participant.relation_id = relation.id unless relation.nil?
  # 	participant.annotation_id = annotation.id unless annotation.nil?
  # 	participant.label = label
  # 	participant.order_no = order
  # 	participant.save

  # 	participant
  # end


end
