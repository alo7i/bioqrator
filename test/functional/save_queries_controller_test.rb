require 'test_helper'

class SaveQueriesControllerTest < ActionController::TestCase
  setup do
    @save_query = save_queries(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:save_queries)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create save_query" do
    assert_difference('SaveQuery.count') do
      post :create, save_query: { collection_id: @save_query.collection_id, duration: @save_query.duration, term: @save_query.term }
    end

    assert_redirected_to save_query_path(assigns(:save_query))
  end

  test "should show save_query" do
    get :show, id: @save_query
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @save_query
    assert_response :success
  end

  test "should update save_query" do
    put :update, id: @save_query, save_query: { collection_id: @save_query.collection_id, duration: @save_query.duration, term: @save_query.term }
    assert_redirected_to save_query_path(assigns(:save_query))
  end

  test "should destroy save_query" do
    assert_difference('SaveQuery.count', -1) do
      delete :destroy, id: @save_query
    end

    assert_redirected_to save_queries_path
  end
end
