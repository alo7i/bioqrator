class CreateUniprotCaches < ActiveRecord::Migration
  def change
    create_table :uniprot_caches do |t|
      t.string :term
      t.text :ret

      t.timestamps
    end
    add_index :uniprot_caches, :term
  end
end
