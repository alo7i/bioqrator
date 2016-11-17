class AddTermInSaveQueryCache < ActiveRecord::Migration
  def up
    add_column :save_query_caches, :term, :string
  end

  def down
  end
end
