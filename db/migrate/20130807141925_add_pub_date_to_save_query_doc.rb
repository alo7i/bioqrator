class AddPubDateToSaveQueryDoc < ActiveRecord::Migration
  def change
    add_column :save_query_docs, :pubdate, :date
    SaveQuery.destroy_all
  end
end
