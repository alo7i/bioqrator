class CreateRelations < ActiveRecord::Migration
  def change
    create_table :relations do |t|
      t.string :type
      t.integer :passage_id
      t.integer :sentence_id
      t.string :r_id
      t.timestamps
    end
  end
end
