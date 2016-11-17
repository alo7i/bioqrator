class CreateSaveQueries < ActiveRecord::Migration
  def change
    create_table :save_queries do |t|
      t.integer :collection_id
      t.string :term
      t.string :duration
      t.timestamps
    end
  end
end
