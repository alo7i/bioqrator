class AddEntityTypesCountToCollection < ActiveRecord::Migration
  def change
    add_column :collections, :entity_types_count, :integer, :default => 0
  end
end
