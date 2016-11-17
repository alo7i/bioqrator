class CreateReferenceLabels < ActiveRecord::Migration
  def change
    create_table :reference_labels do |t|
      t.string :label
      t.integer :reference_id

      t.timestamps
    end
  end
end
