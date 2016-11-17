class CreateTypeFromAnnotation < ActiveRecord::Migration
  def up
  	add_index :entity_types, [:collection_id, :name]

  	Collection.all.each do |c|
  		c.documents.each do |d|
  			d.annotations.each do |a|
  				EntityType.find_or_create(c, a.atype)
  			end

  			d.relations.each do |r|
					EntityType.find_or_create(c, r.rtype)
  			end
  		end
  	end
  end

  def down
  end
end
