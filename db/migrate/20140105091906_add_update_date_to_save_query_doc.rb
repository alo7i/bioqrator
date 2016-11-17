class AddUpdateDateToSaveQueryDoc < ActiveRecord::Migration
  def change
    add_column :save_query_docs, :update_date, :date
  end
end
