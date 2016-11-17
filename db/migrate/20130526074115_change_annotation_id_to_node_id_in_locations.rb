class ChangeAnnotationIdToNodeIdInLocations < ActiveRecord::Migration
  def up
  	rename_column :locations, :annotation_id, :node_id
  	remove_column :nodes, :entity_key
  	remove_column :nodes, :entity_type_id
  	add_column :nodes, :locations_count, :integer, :default => 0
  	add_column :nodes, :references_count, :integer, :default => 0

  	remove_column :passages, :ptype
  	remove_column :documents, :authors
  	remove_column :collections, :entity_types_count
  end

  def down
  end
end
