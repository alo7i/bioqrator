class AddPpiScoreToDocument < ActiveRecord::Migration
  def change
    add_column :documents, :ppi_score, :float, :default => 0
    add_column :documents, :pub_date, :date
  end
end
