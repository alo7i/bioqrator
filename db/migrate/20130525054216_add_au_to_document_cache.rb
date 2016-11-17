class AddAuToDocumentCache < ActiveRecord::Migration
  def change
    add_column :document_caches, :au, :string
  end
end
