class AddLabelToParticipants < ActiveRecord::Migration
  def change
    add_column :participants, :label, :string
    remove_column :participants, :participant_in
  end
end
