class CreatePubmedQueries < ActiveRecord::Migration
  def change
    create_table :pubmed_queries do |t|
      t.string :term
      t.integer :count, :default => 0

      t.timestamps
    end
    add_index :pubmed_queries, :term
  end
end
