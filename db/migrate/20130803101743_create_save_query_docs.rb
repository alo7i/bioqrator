class CreateSaveQueryDocs < ActiveRecord::Migration
  def change
    create_table :save_query_docs do |t|
      t.string :pmid
      t.float :score
      t.integer :save_query_cache_id

      t.timestamps
    end
  end
end
