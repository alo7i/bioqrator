class CreateRecommenders < ActiveRecord::Migration
  def change
    create_table :recommenders do |t|
      t.string :name
      t.string :link
      t.string :desc
      t.integer :collection_id
      t.string :type_str

      t.timestamps
    end
  end
end
