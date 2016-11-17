class CreateTypeProperties < ActiveRecord::Migration
  def change
    create_table :type_properties do |t|
      t.integer :entity_type_id
      t.string :name
      t.string :category
      t.text :values

      t.timestamps
    end

    add_column :entity_types, :type_properties_count, :integer, :default => 0
  end
end
