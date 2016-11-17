class AddParticipatesInToParticipants < ActiveRecord::Migration
  def change
    add_column :participants, :participates_in, :integer
  end
end
