class Annotation < ActiveRecord::Base
  # attr_accessible :atext, :length, :offset, :passage_id, :aid, :document_id, :entity_type_id, :entity_key
  # belongs_to :document, :counter_cache => true
  # belongs_to :passage
  # has_many :locations, :dependent => :destroy, :order => "offset, length"
  # belongs_to :entity_type
  
  # validates :aid, :presence => true
  # before_save :check_unique_aid


  # def check_unique_aid
  #   self.document.annotations.each do |a|
  #     if a.aid == self.aid && ( self.id != nil && a.id != self.id)
  #       errors[:aid] = "Same annotation id exists"
  #       return false
  #     end
  #   end
  #   self.document.relations.each do |r|
  #     if self.aid == r.rid
  #       errors[:aid] = "Same relation id exists"
  #     end
  #   end   
  # end

  # def desc
  # 	"#{atext} (#{offset}:#{length})"
  # end

  # def type_str 
  #   if self.entity_type.nil?
  #     "-"
  #   else
  #     self.entity_type.name 
  #   end
  # end

  # def create_location(locstr)
  # 	locstr.split(";").each do |location|
  # 		no = location.split(":").map{|t| t.strip.to_i}
  # 		find_or_create_location(no[0], no[1])
  # 	end
  # end

 	# def find_or_create_location(offset, length)
 	# 	loc = locations.where("offset=? and length=?", offset, length).first
 	# 	if loc.nil?
  # 		locations.create({:offset => offset, :length => length})
 	# 	end
 	# end

  # def position
  #   if locations.empty?
  #     0
  #   else
  #     locations.first.offset
  #   end
  # end

  # def locstr
  # 	locations.map{|l| "#{l.offset}:#{l.length}"}.join(";")
  # end

  # def locstr_with_br(ranges)
  #   ret = []
  #   locations.each do |l|
  #     if l.has_overlap?(ranges)
  #       ret << "<span style='text-decoration:line-through' title='overlapped location cannot be displayed!'>#{l.offset}:#{l.length}</span>"
  #     else
  #       ret << "#{l.offset}:#{l.length}"
  #       ranges << [l.offset, l.length]
  #     end
  #   end
  #   ret.join("<br>").html_safe
  # end
end
