class User < ActiveRecord::Base
  # Include default devise modules. Others available are:
  # :token_authenticatable, :confirmable,
  # :lockable, :timeoutable and :omniauthable
  devise :database_authenticatable, 
         :registerable,
         :recoverable, :rememberable, :trackable, :validatable

  # Setup accessible (or protected) attributes for your model
  attr_accessible :email, :password, :password_confirmation, :remember_me, :first_name, :last_name
  # attr_accessible :title, :body

  has_many :own_collections, :foreign_key => :owner_id, :class_name => "Collection", :dependent => :destroy

  has_many :user_collections
  has_many :collections, :through => :user_collections
  


  def name
    [first_name, last_name].join(" ")
  end

  def name_or_email
  	if self.name.blank?
  		return email
  	end

  	return name
  end
end
