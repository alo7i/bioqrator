class AddDocumentCountToCollection < ActiveRecord::Migration
  def change
    add_column :collections, :documents_count, :integer, :default => 0
    Collection.reset_column_information
    Collection.find_each do |collection|
    	Collection.reset_counters collection.id, :documents
    end

    add_column :documents, :annotations_count, :integer, :default => 0
    add_column :documents, :relations_count, :integer, :default => 0

    Document.reset_column_information
    Document.find_each do |document|
      Document.reset_counters document.id, :annotations
      Document.reset_counters document.id, :relations
    end

  end
end
