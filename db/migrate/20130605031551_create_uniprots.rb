class CreateUniprots < ActiveRecord::Migration
  def change
    create_table :uniprots do |t|
      t.string :uniprot_id
      t.string :entry_name
      t.text :protein_names
      t.string :gene_name
      t.string :organism
      t.integer :length

      t.timestamps
    end

    add_index :uniprots, :uniprot_id
  end
end
