class CreateNodes < ActiveRecord::Migration
  def change
    create_table :nodes do |t|
      t.string :node_text
      t.string :entity_key
      t.integer :entity_type_id
      t.string :node_id
      t.string :node_type
      t.integer :document_id
      t.integer :passage_id

      t.timestamps
    end
  end
end
