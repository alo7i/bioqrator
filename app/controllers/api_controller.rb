class ApiController < ApplicationController
  def uniprot
    @term = params[:term]
    @ret = UniprotQuery.query(@term).sort
    render :layout => nil
  end

  def fetch_uniprot
    u = Uniprot.fetch(params[:id])
    if u.nil?
      render :text => ""
    else
      render :text => u.desc.html_safe()
    end
  end
  
  def entrez
    @term = params[:term]
    @term2 = @term.gsub(/[^0-9a-z ]/i, '')
    @genes = EntrezGene.find_by_sql([
      "SELECT DISTINCT g.id, g.description, g.name, g.species
            FROM entrez_genes g JOIN entrez_synonyms s on g.id = s.entrez_gene_id
            WHERE s.name in (?) 
            ORDER BY g.species_priority DESC, g.name ASC", 
      [@term, @term2].uniq
    ])
    render :layout => nil
  end

  def update_document
    @pmid = params[:pmid]
    

  end
end
