class AddChildIdToReference < ActiveRecord::Migration
  def change
    add_column :references, :child_id, :integer

    Reference.all.each do |r|
      next if r.node.nil?
      next if r.node.document.nil?
      
      a = r.node.document.nodes.where("node_id = ?", r.ref_id).first
      unless a.nil?
        r.child_id = a.id
        r.save
      end
    end
  end
end
