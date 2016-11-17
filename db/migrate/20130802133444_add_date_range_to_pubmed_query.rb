class AddDateRangeToPubmedQuery < ActiveRecord::Migration
  def change
    add_column :pubmed_queries, :date_range, :string
  end
end
