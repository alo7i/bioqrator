class CreateCollections < ActiveRecord::Migration
  def change
    create_table :collections do |t|
      t.string :name
      t.string :source
      t.string :key
      t.string :s_date
      t.integer :owner_id

      t.timestamps
    end
  end
end
