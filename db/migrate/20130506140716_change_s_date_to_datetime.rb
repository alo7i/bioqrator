class ChangeSDateToDatetime < ActiveRecord::Migration
  def up
  	change_column :collections, :s_date, :date
  end

  def down
  end
end
