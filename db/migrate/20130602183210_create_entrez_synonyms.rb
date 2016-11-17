class CreateEntrezSynonyms < ActiveRecord::Migration
  def change
    create_table :entrez_synonyms do |t|
      t.integer :entrez_gene_id
      t.string :name

      t.timestamps
    end

    add_index :entrez_synonyms, :entrez_gene_id
    add_index :entrez_synonyms, :name
  end
end
