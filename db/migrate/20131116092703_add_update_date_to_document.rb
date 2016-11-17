class AddUpdateDateToDocument < ActiveRecord::Migration
  def change
    add_column :documents, :update_date, :string
    add_column :document_caches, :update_date, :string
  end
end
