class RenameRelationId < ActiveRecord::Migration
  def change
  	rename_column :relations, :r_id, :rid
  end

end
