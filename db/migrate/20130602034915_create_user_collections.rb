class CreateUserCollections < ActiveRecord::Migration
  def change
    create_table :user_collections do |t|
      t.integer :user_id
      t.integer :collection_id

      t.timestamps
    end
  end
end
