class CreatePassages < ActiveRecord::Migration
  def change
    create_table :passages do |t|
      t.string :type
      t.integer :offset
      t.integer :document_id

      t.timestamps
    end
  end
end
