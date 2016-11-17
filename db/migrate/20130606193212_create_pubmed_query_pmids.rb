class CreatePubmedQueryPmids < ActiveRecord::Migration
  def change
    create_table :pubmed_query_pmids do |t|
      t.integer :pmid
      t.float :pie_score
      t.integer :pubmed_order
      t.integer :pubmed_query_id
      t.timestamps
    end
    add_index :pubmed_query_pmids, :pubmed_query_id
    add_index :pubmed_query_pmids, [:pubmed_query_id, :pubmed_order]
    add_index :pubmed_query_pmids, [:pubmed_query_id, :pie_score]
  end
end
