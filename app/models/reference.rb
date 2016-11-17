class Reference < ActiveRecord::Base
  attr_accessible :node_id, :order_no, :ref_id, :role, :child_id

  belongs_to :node, :counter_cache => true
  belongs_to :child, :class_name => "Node", :foreign_key => "child_id", :primary_key => "id"

  def restore_child
    self.node.document.nodes.each do |n|
      if self.ref_id == n.node_id
        self.child = n
        self.save
      end
    end
  end
end
