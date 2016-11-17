class AddMedlinePmidToDocumentCache < ActiveRecord::Migration
  def change
    add_column :document_caches, :medline, :text
    add_column :document_caches, :pmid, :integer
  end
end
