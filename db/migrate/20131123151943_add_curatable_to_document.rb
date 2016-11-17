class AddCuratableToDocument < ActiveRecord::Migration
  def change
    add_column :documents, :curatable, :boolean, :default => true
    change_column :documents, :done, :boolean, :default => false
  end
end
