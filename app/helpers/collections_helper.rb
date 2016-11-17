module CollectionsHelper
	def link_to_user(user)
		link_to user.email, user unless user.nil?
	end
end
