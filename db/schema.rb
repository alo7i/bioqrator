# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended to check this file into your version control system.

ActiveRecord::Schema.define(:version => 20140105100101) do

  create_table "annotations", :force => true do |t|
    t.string   "atype"
    t.string   "atext"
    t.integer  "passage_id"
    t.integer  "sentence_id"
    t.string   "aid"
    t.datetime "created_at",                     :null => false
    t.datetime "updated_at",                     :null => false
    t.integer  "document_id"
    t.integer  "locations_count", :default => 0
    t.string   "entity_key"
    t.integer  "entity_type_id"
  end

  add_index "annotations", ["entity_type_id"], :name => "index_annotations_on_entity_type_id"

  create_table "collections", :force => true do |t|
    t.string   "name"
    t.string   "source"
    t.string   "key"
    t.date     "s_date"
    t.integer  "owner_id"
    t.datetime "created_at",                     :null => false
    t.datetime "updated_at",                     :null => false
    t.integer  "documents_count", :default => 0
  end

  add_index "collections", ["owner_id"], :name => "index_collections_on_owner_id"

  create_table "document_caches", :force => true do |t|
    t.string   "source"
    t.text     "abstract"
    t.string   "authors"
    t.string   "year"
    t.string   "title"
    t.datetime "created_at",  :null => false
    t.datetime "updated_at",  :null => false
    t.string   "au"
    t.text     "medline"
    t.integer  "pmid"
    t.float    "ppi_score"
    t.date     "pubdate"
    t.date     "update_date"
  end

  create_table "documents", :force => true do |t|
    t.string   "pmid"
    t.integer  "collection_id"
    t.string   "source"
    t.text     "abstract"
    t.integer  "year"
    t.text     "medline"
    t.datetime "created_at",                           :null => false
    t.datetime "updated_at",                           :null => false
    t.string   "title"
    t.string   "au"
    t.integer  "annotations_count", :default => 0
    t.integer  "relations_count",   :default => 0
    t.float    "ppi_score"
    t.date     "pub_date"
    t.boolean  "done",              :default => false
    t.string   "update_date"
    t.boolean  "curatable",         :default => true
  end

  add_index "documents", ["collection_id"], :name => "index_documents_on_collection_id"

  create_table "entity_types", :force => true do |t|
    t.string   "name"
    t.string   "url"
    t.string   "nomenclature"
    t.string   "category"
    t.integer  "collection_id"
    t.datetime "created_at",                              :null => false
    t.datetime "updated_at",                              :null => false
    t.integer  "type_properties_count", :default => 0
    t.boolean  "for_entity",            :default => true
    t.boolean  "for_relation",          :default => true
  end

  add_index "entity_types", ["collection_id", "name"], :name => "index_entity_types_on_collection_id_and_name"

  create_table "entrez_genes", :force => true do |t|
    t.string   "name"
    t.string   "description"
    t.string   "species"
    t.datetime "created_at",                        :null => false
    t.datetime "updated_at",                        :null => false
    t.integer  "species_priority", :default => 999
  end

  add_index "entrez_genes", ["name"], :name => "index_entrez_genes_on_name"

  create_table "entrez_synonyms", :force => true do |t|
    t.integer  "entrez_gene_id"
    t.string   "name"
    t.datetime "created_at",     :null => false
    t.datetime "updated_at",     :null => false
  end

  add_index "entrez_synonyms", ["entrez_gene_id"], :name => "index_entrez_synonyms_on_entrez_gene_id"
  add_index "entrez_synonyms", ["name"], :name => "index_entrez_synonyms_on_name"

  create_table "info_nodes", :force => true do |t|
    t.string   "title"
    t.string   "key"
    t.string   "node_type"
    t.integer  "collection_id"
    t.integer  "document_id"
    t.integer  "passage_id"
    t.integer  "node_id"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  add_index "info_nodes", ["collection_id"], :name => "index_info_nodes_on_collection_id"
  add_index "info_nodes", ["document_id"], :name => "index_info_nodes_on_document_id"
  add_index "info_nodes", ["node_id"], :name => "index_info_nodes_on_node_id"
  add_index "info_nodes", ["passage_id"], :name => "index_info_nodes_on_passage_id"

  create_table "locations", :force => true do |t|
    t.integer  "node_id"
    t.integer  "offset"
    t.integer  "length"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  add_index "locations", ["node_id"], :name => "index_locations_on_node_id"

  create_table "negative_documents", :force => true do |t|
    t.integer  "collection_id"
    t.integer  "pmid"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  add_index "negative_documents", ["collection_id", "pmid"], :name => "index_negative_documents_on_collection_id_and_pmid", :unique => true

  create_table "nodes", :force => true do |t|
    t.string   "node_text"
    t.string   "node_id"
    t.string   "node_type"
    t.integer  "document_id"
    t.integer  "passage_id"
    t.datetime "created_at",                         :null => false
    t.datetime "updated_at",                         :null => false
    t.integer  "locations_count",     :default => 0
    t.integer  "references_count",    :default => 0
    t.string   "entity_type"
    t.string   "representative_name"
  end

  add_index "nodes", ["document_id", "node_type"], :name => "index_nodes_on_document_id_and_node_type"
  add_index "nodes", ["document_id"], :name => "index_nodes_on_document_id"
  add_index "nodes", ["passage_id", "node_type"], :name => "index_nodes_on_passage_id_and_node_type"
  add_index "nodes", ["passage_id"], :name => "index_nodes_on_passage_id"

  create_table "participants", :force => true do |t|
    t.integer  "ptype"
    t.integer  "relation_id"
    t.integer  "annotation_id"
    t.integer  "order_no"
    t.datetime "created_at",      :null => false
    t.datetime "updated_at",      :null => false
    t.string   "label"
    t.integer  "participates_in"
  end

  create_table "passages", :force => true do |t|
    t.integer  "offset"
    t.integer  "document_id"
    t.datetime "created_at",   :null => false
    t.datetime "updated_at",   :null => false
    t.text     "ptext"
    t.string   "passage_type"
  end

  add_index "passages", ["document_id"], :name => "index_passages_on_document_id"

  create_table "pie_articles", :force => true do |t|
    t.text     "title"
    t.text     "abstract"
    t.integer  "pub_year"
    t.date     "pub_date"
    t.string   "source"
    t.string   "authors"
    t.float    "ppi_score"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  create_table "pie_entities", :force => true do |t|
    t.string   "name"
    t.integer  "pmid"
    t.integer  "entrez_id"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  add_index "pie_entities", ["pmid"], :name => "index_pie_entities_on_pmid"

  create_table "pie_entity_locations", :force => true do |t|
    t.integer  "pie_entity_id"
    t.integer  "offset"
    t.integer  "length"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  add_index "pie_entity_locations", ["pie_entity_id"], :name => "index_pie_entity_locations_on_pie_entity_id"

  create_table "pubmed_pie_scores", :force => true do |t|
    t.float    "pie_score"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  create_table "pubmed_queries", :force => true do |t|
    t.string   "term"
    t.integer  "count",      :default => 0
    t.datetime "created_at",                :null => false
    t.datetime "updated_at",                :null => false
    t.string   "date_range"
  end

  add_index "pubmed_queries", ["term"], :name => "index_pubmed_queries_on_term"

  create_table "pubmed_query_pmids", :force => true do |t|
    t.integer  "pmid"
    t.float    "pie_score"
    t.integer  "pubmed_order"
    t.integer  "pubmed_query_id"
    t.datetime "created_at",      :null => false
    t.datetime "updated_at",      :null => false
  end

  add_index "pubmed_query_pmids", ["pubmed_query_id", "pie_score"], :name => "index_pubmed_query_pmids_on_pubmed_query_id_and_pie_score"
  add_index "pubmed_query_pmids", ["pubmed_query_id", "pubmed_order"], :name => "index_pubmed_query_pmids_on_pubmed_query_id_and_pubmed_order"
  add_index "pubmed_query_pmids", ["pubmed_query_id"], :name => "index_pubmed_query_pmids_on_pubmed_query_id"

  create_table "recommenders", :force => true do |t|
    t.string   "name"
    t.string   "link"
    t.string   "desc"
    t.integer  "collection_id"
    t.string   "type_str"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  create_table "reference_labels", :force => true do |t|
    t.string   "label"
    t.integer  "reference_id"
    t.datetime "created_at",   :null => false
    t.datetime "updated_at",   :null => false
  end

  create_table "references", :force => true do |t|
    t.string   "ref_id"
    t.string   "role"
    t.integer  "node_id"
    t.integer  "order_no"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
    t.integer  "child_id"
  end

  add_index "references", ["node_id"], :name => "index_references_on_node_id"
  add_index "references", ["ref_id"], :name => "index_references_on_ref_id"

  create_table "relations", :force => true do |t|
    t.string   "rtype"
    t.integer  "passage_id"
    t.integer  "sentence_id"
    t.string   "rid"
    t.datetime "created_at",     :null => false
    t.datetime "updated_at",     :null => false
    t.integer  "document_id"
    t.string   "entity_key"
    t.integer  "entity_type_id"
  end

  add_index "relations", ["entity_type_id"], :name => "index_relations_on_entity_type_id"

  create_table "save_queries", :force => true do |t|
    t.integer  "collection_id"
    t.string   "term"
    t.string   "duration"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  create_table "save_query_caches", :force => true do |t|
    t.string   "duration"
    t.integer  "save_query_id"
    t.integer  "save_query_docs_count", :default => 0
    t.datetime "cache_date"
    t.datetime "created_at",                           :null => false
    t.datetime "updated_at",                           :null => false
    t.string   "term"
  end

  create_table "save_query_docs", :force => true do |t|
    t.string   "pmid"
    t.float    "score"
    t.integer  "save_query_cache_id"
    t.datetime "created_at",          :null => false
    t.datetime "updated_at",          :null => false
    t.date     "pubdate"
    t.date     "update_date"
  end

  create_table "sentences", :force => true do |t|
    t.integer  "offset"
    t.text     "content"
    t.integer  "passage_id"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  create_table "tests", :force => true do |t|
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  create_table "type_properties", :force => true do |t|
    t.integer  "entity_type_id"
    t.string   "name"
    t.string   "category"
    t.text     "values"
    t.datetime "created_at",     :null => false
    t.datetime "updated_at",     :null => false
  end

  create_table "uniprot_caches", :force => true do |t|
    t.string   "term"
    t.text     "ret"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  add_index "uniprot_caches", ["term"], :name => "index_uniprot_caches_on_term"

  create_table "uniprot_queries", :force => true do |t|
    t.string   "term"
    t.string   "uniprot_id"
    t.integer  "order_no"
    t.datetime "created_at", :null => false
    t.datetime "updated_at", :null => false
  end

  add_index "uniprot_queries", ["term"], :name => "index_uniprot_queries_on_term"

  create_table "uniprots", :force => true do |t|
    t.string   "uniprot_id"
    t.string   "entry_name"
    t.text     "protein_names"
    t.string   "gene_name"
    t.string   "organism"
    t.integer  "length"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  add_index "uniprots", ["uniprot_id"], :name => "index_uniprots_on_uniprot_id"

  create_table "user_collections", :force => true do |t|
    t.integer  "user_id"
    t.integer  "collection_id"
    t.datetime "created_at",    :null => false
    t.datetime "updated_at",    :null => false
  end

  add_index "user_collections", ["collection_id"], :name => "index_user_collections_on_collection_id"
  add_index "user_collections", ["user_id"], :name => "index_user_collections_on_user_id"

  create_table "users", :force => true do |t|
    t.string   "email",                  :default => "", :null => false
    t.string   "encrypted_password",     :default => "", :null => false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          :default => 0
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.datetime "created_at",                             :null => false
    t.datetime "updated_at",                             :null => false
    t.string   "last_name"
    t.string   "first_name"
  end

  add_index "users", ["email"], :name => "index_users_on_email", :unique => true
  add_index "users", ["reset_password_token"], :name => "index_users_on_reset_password_token", :unique => true

end
