require 'test_helper'

class RecommendersControllerTest < ActionController::TestCase
  setup do
    @recommender = recommenders(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:recommenders)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create recommender" do
    assert_difference('Recommender.count') do
      post :create, recommender: { collection_id: @recommender.collection_id, desc: @recommender.desc, link: @recommender.link, name: @recommender.name, type_str: @recommender.type_str }
    end

    assert_redirected_to collection_recommender_path(@collection, assigns(:recommender))
  end

  test "should show recommender" do
    get :show, id: @recommender
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @recommender
    assert_response :success
  end

  test "should update recommender" do
    put :update, id: @recommender, recommender: { collection_id: @recommender.collection_id, desc: @recommender.desc, link: @recommender.link, name: @recommender.name, type_str: @recommender.type_str }
    assert_redirected_to recommender_path(assigns(:recommender))
  end

  test "should destroy recommender" do
    assert_difference('Recommender.count', -1) do
      delete :destroy, id: @recommender
    end

    assert_redirected_to recommenders_path
  end
end
