class CreatePieArticles < ActiveRecord::Migration
  def change
    create_table :pie_articles do |t|
      t.text :title
      t.text :abstract
      t.integer :pub_year
      t.date :pub_date
      t.string :source
      t.string :authors
      t.float :ppi_score

      t.timestamps
    end
  end
end
