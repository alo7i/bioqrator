class AddPriorityToEntrezGene < ActiveRecord::Migration
  def change
    add_column :entrez_genes, :species_priority, :integer, :default => 999
    EntrezGene.connection.execute("update entrez_genes set species_priority = 1 where species = 'Homo sapiens'");
    EntrezGene.connection.execute("update entrez_genes set species_priority = 10 where species = 'Mus musculus'");
    EntrezGene.connection.execute("update entrez_genes set species_priority = 20 where species = 'Rattus norvegicus'");
  end
end
