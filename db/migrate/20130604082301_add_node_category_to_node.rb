class AddNodeCategoryToNode < ActiveRecord::Migration
  def change
    add_column :nodes, :entity_type, :string    
    add_column :passages, :passage_type, :string
  end
end
