class CreatePieEntityLocations < ActiveRecord::Migration
  def change
    create_table :pie_entity_locations do |t|
      t.integer :pie_entity_id
      t.integer :offset
      t.integer :length

      t.timestamps
    end
  end
end
