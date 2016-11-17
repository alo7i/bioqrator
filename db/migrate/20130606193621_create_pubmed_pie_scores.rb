class CreatePubmedPieScores < ActiveRecord::Migration
  def change
    create_table :pubmed_pie_scores do |t|
      t.float :pie_score

      t.timestamps
    end
  end
end
