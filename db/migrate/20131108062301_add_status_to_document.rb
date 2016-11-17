class AddStatusToDocument < ActiveRecord::Migration
  def change
    add_column :documents, :done, :boolean, :default =>false
  end
end
