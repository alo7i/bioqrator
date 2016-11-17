class CreateDocumentCaches < ActiveRecord::Migration
  def change
    create_table :document_caches do |t|
      t.string :id
      t.string :source
      t.text :abstract
      t.string :authors
      t.string :year
      t.string :title

      t.timestamps
    end
  end
end
