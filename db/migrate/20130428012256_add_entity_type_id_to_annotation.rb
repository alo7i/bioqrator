class AddEntityTypeIdToAnnotation < ActiveRecord::Migration
  def change
    add_column :annotations, :entity_type_id, :integer
    add_index :annotations, :entity_type_id
  end
end
