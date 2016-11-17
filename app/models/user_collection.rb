class UserCollection < ActiveRecord::Base
  attr_accessible :collection_id, :user_id
  belongs_to :user
  belongs_to :collection
end
