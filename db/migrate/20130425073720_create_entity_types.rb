class CreateEntityTypes < ActiveRecord::Migration
  def change
    create_table :entity_types do |t|
      t.string :name
      t.string :url
      t.string :nomenclature
      t.string :category
      t.integer :collection_id

      t.timestamps
    end
  end
end
