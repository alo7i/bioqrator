class AddEntityKeyToRelation < ActiveRecord::Migration
  def change
    add_column :relations, :entity_key, :string
    add_column :annotations, :entity_key, :string
  end
end
