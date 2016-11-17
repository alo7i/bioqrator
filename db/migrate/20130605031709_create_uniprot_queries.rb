class CreateUniprotQueries < ActiveRecord::Migration
  def change
    create_table :uniprot_queries do |t|
      t.string :term
      t.string :uniprot_id
      t.integer :order_no

      t.timestamps
    end
    add_index :uniprot_queries, :term
  end
end
