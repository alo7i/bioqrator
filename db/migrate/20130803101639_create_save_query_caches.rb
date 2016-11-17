class CreateSaveQueryCaches < ActiveRecord::Migration
  def change
    create_table :save_query_caches do |t|
      t.string :duration
      t.integer :save_query_id
      t.integer :save_query_doc_count
      t.date :cache_date

      t.timestamps
    end
  end
end
