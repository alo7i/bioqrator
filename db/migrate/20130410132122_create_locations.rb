class CreateLocations < ActiveRecord::Migration
  def change
    create_table :locations do |t|
      t.integer :annotation_id
      t.integer :offset
      t.integer :length

      t.timestamps
    end
  	add_column :annotations, :locations_count, :integer, :default => 0

    Annotation.find_each do |annotation|
    	annotation.locations.create({:offset => annotation.offset, :length => annotation.length});
    end

    remove_column :annotations, :offset
    remove_column :annotations, :length
  end
end
