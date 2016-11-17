class CreateParticipants < ActiveRecord::Migration
  def change
    create_table :participants do |t|
      t.integer :participant_in
      t.integer :type
      t.integer :reference_id
      t.integer :annotation_id
      t.integer :order

      t.timestamps
    end
  end
end
