module ApplicationHelper
	def class_for_active controller_names
		if controller_names.include? params[:controller] 
			"class='active'".html_safe
		elsif controller_name.include? "#{params[:controller]}/#{params[:action]}"
			"class='active'".html_safe
		else
		end
	end
end
