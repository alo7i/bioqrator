class AddScoreToDocument < ActiveRecord::Migration
  def change
    add_column :document_caches, :ppi_score, :float
    add_column :document_caches, :pubdate, :date
    DocumentCache.destroy_all
  end
end
