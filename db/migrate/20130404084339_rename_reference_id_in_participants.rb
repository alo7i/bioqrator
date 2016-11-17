class RenameReferenceIdInParticipants < ActiveRecord::Migration
  def change
  	rename_column :participants, :reference_id, :relation_id
  end
end
