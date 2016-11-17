class AddIndexToPmid < ActiveRecord::Migration
  def change
  	add_index :pie_entities, :pmid
  	add_index :collections, :owner_id
  	add_index :user_collections, :user_id
  	add_index :user_collections, :collection_id
  	add_index :references, :ref_id
  	add_index :references, :node_id
  	add_index :pie_entity_locations, :pie_entity_id
  	add_index :passages, :document_id
  	add_index :nodes, :document_id
  	add_index :nodes, :passage_id
  	add_index :nodes, [:document_id, :node_type]
  	add_index :nodes, [:passage_id, :node_type]
  	add_index :info_nodes, :node_id
  	add_index :info_nodes, :passage_id
  	add_index :info_nodes, :document_id
  	add_index :info_nodes, :collection_id
  	add_index :locations, :node_id
  	add_index :documents, :collection_id
  end
end
