class ChangeCacheTimeInSaveQueryCaches < ActiveRecord::Migration
  def up
    change_column :save_query_caches, :cache_date, :datetime
  end

  def down
  end
end
