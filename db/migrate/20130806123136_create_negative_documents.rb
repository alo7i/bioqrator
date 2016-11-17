class CreateNegativeDocuments < ActiveRecord::Migration
  def change
    create_table :negative_documents do |t|
      t.integer :collection_id
      t.integer :pmid

      t.timestamps
    end
    add_index :negative_documents, [:collection_id, :pmid], :unique => true
  end
end
