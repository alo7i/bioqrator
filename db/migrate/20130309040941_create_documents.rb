class CreateDocuments < ActiveRecord::Migration
  def change
    create_table :documents do |t|
      t.string :pmid
      t.integer :collection_id
      t.string :source
      t.text :abstract
      t.string :authors
      t.integer :year
      t.text :medline
      t.timestamps
    end
  end
end
