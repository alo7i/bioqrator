class CreateSentences < ActiveRecord::Migration
  def change
    create_table :sentences do |t|
      t.integer :offset
      t.text :content
      t.integer :passage_id

      t.timestamps
    end
  end
end
