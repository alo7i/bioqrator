class AddDocStatusToDoc < ActiveRecord::Migration
  def change
    change_column :documents, :done, :integer, :default => 0
  end
end
