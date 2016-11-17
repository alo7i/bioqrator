class AdminController < ApplicationController
  before_filter :authenticate_basic
  before_filter :authenticate_user!

  def index
  end

  def refresh
    DocumentCache.delete_all
    Uniprot.delete_all
    UniprotQuery.delete_all
    SaveQueryCache.delete_all
    SaveQueryDoc.delete_all
  end

  protected
    def authenticate_basic
      authenticate_or_request_with_http_basic do |username, password|
      username == "admin" && password == "dkffkdrkahffk!"
    end
  end
end
