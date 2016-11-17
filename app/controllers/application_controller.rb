class ApplicationController < ActionController::Base
  protect_from_forgery

  def after_sign_in_path_for(resource)
   collections_path
  end

  def baseCollections
    # super_admins = ["dongseop@gmail.com", "sun.kim@nih.gov", "sooyong.shin@gmail.com"]
    super_admins = ["bioqrator-admin@gmail.com"]
    if super_admins.include?(current_user.email) 
      return Collection
    else
      return current_user.collections
    end
  end
end
