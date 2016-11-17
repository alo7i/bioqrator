class CreateAnnotations < ActiveRecord::Migration
  def change
    create_table :annotations do |t|
      t.string :type
      t.integer :offset
      t.integer :length
      t.string :atext
      t.integer :passage_id
      t.integer :sentence_id
      t.string :aid
      t.timestamps
    end
  end
end
