class ChangeTypeForUpdateDateofDocumentCache < ActiveRecord::Migration
  def up
    change_column :document_caches, :update_date, :date
  end

  def down
    change_column :document_caches, :update_date, :date
  end
end
