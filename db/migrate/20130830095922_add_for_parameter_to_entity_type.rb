class AddForParameterToEntityType < ActiveRecord::Migration
  def change
    add_column :entity_types, :for_entity, :boolean, :default => true
    add_column :entity_types, :for_relation, :boolean, :default => true
  end
end
