class DropDefaultPpiScoreToDocument < ActiveRecord::Migration
  def up
    change_column_default(:documents, :ppi_score, nil)
  end

  def down
  end
end
