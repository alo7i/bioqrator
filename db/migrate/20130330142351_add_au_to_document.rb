class AddAuToDocument < ActiveRecord::Migration
  def change
    add_column :documents, :au, :string
  end
end
