class AddDocumentIdToAnnotation < ActiveRecord::Migration
  def change
    add_column :annotations, :document_id, :integer
    add_column :relations, :document_id, :integer
    add_column :passages, :ptext, :text
    rename_column :participants, :type, :ptype
    rename_column :participants, :order, :order_no
    rename_column :relations, :type, :rtype
    rename_column :annotations, :type, :atype
    rename_column :passages, :type, :ptype

  end
end
