class AddEntityTypeIdToRelation < ActiveRecord::Migration
  def change
    add_column :relations, :entity_type_id, :integer
    add_index :relations, :entity_type_id
  end
end
