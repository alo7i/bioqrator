class ChangeSaveQueryDocCounterInSaveQueryCache < ActiveRecord::Migration
  def up
    change_column :save_query_caches, :save_query_doc_count, :integer, :default => 0
    rename_column :save_query_caches, :save_query_doc_count, :save_query_docs_count
  end

  def down
  end
end
