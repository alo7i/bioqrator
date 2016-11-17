class CreateInfoNodes < ActiveRecord::Migration
  def change
    create_table :info_nodes do |t|
      t.string :title
      t.string :key
      t.string :node_type
      t.integer :collection_id
      t.integer :document_id
      t.integer :passage_id
      t.integer :node_id

      t.timestamps
    end
  end
end
