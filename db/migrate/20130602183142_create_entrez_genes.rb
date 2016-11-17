class CreateEntrezGenes < ActiveRecord::Migration
  def change
    create_table :entrez_genes do |t|
      t.string :name
      t.string :description
      t.string :species

      t.timestamps
    end

    add_index :entrez_genes, :name
  end
end
