class Participant < ActiveRecord::Base
  attr_accessible :annotation_id, :order_no, :relation_id, :ptype, :label, :participates_in
  belongs_to :annotation
  belongs_to :relation
  belongs_to :parent_relation, :class_name => "Relation", :foreign_key => "participates_in"

  def ref_id
  	if annotation.nil? && !relation.nil?
  		relation.rid
  	elsif !annotation.nil? && relation.nil?
  		annotation.aid
  	else
  		nil
  	end
  end

  def desc
    if annotation.nil? && !relation.nil?
      relation.desc
    elsif !annotation.nil? && relation.nil?
      annotation.desc
    else
      nil
    end
  end
end
