class AddRepNameToNode < ActiveRecord::Migration
  def change
    add_column :nodes, :representative_name, :string
  end
end
