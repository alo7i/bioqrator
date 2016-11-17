require 'test_helper'

class ApiControllerTest < ActionController::TestCase
  test "should get uniprot" do
    get :uniprot
    assert_response :success
  end

  test "should get entrez" do
    get :entrez
    assert_response :success
  end

end
