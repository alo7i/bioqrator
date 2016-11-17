class CreateReferences < ActiveRecord::Migration
  def change
    create_table :references do |t|
      t.string :ref_id
      t.string :role
      t.integer :node_id
      t.integer :order_no

      t.timestamps
    end
  end
end
