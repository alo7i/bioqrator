class UsersController  < ApplicationController

	before_filter :authenticate, :except => "show"

	def index
		@users = User.page(params[:page])
		respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @users }
    end
	end

  def show
    @user = User.find(params[:id])
  end

  def destroy
    @user = User.find(params[:id])
    @user.own_collections.each do |c|
      c.destroy_bulk
    end
    
    @user.destroy

    respond_to do |format|
      format.html { redirect_to users_url, notice: 'User was successfully removed.'}
      format.json { head :no_content }
    end
  end

	protected
    def authenticate
      authenticate_or_request_with_http_basic do |username, password|
        username == "admin" && password == "dkffkdrkahffk!"
      end
    end  
end