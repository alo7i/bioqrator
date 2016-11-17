class CreatePieEntities < ActiveRecord::Migration
  def change
    create_table :pie_entities do |t|
      t.string :name
      t.integer :pmid
      t.integer :entrez_id

      t.timestamps
    end
  end
end
