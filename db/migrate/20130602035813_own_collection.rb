class OwnCollection < ActiveRecord::Migration
  def up
  	Collection.all.each do |c|
  		UserCollection.create({:collection_id => c.id, :user_id => c.owner_id})
  	end
  end

  def down
  end
end
