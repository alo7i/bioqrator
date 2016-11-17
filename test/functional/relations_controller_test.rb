require 'test_helper'

class RelationsControllerTest < ActionController::TestCase
  setup do
    @relation = relations(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:relations)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create relation" do
    assert_difference('Relation.count') do
      post :create, relation: { entity_key: @relation.entity_key}
    end

    assert_redirected_to relation_path(assigns(:relation))
  end

  test "should show relation" do
    get :show, id: @relation
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @relation
    assert_response :success
  end

  test "should update relation" do
    put :update, id: @relation, relation: { entity_key: @relation.entity_key }
    assert_redirected_to relation_path(assigns(:relation))
  end

  test "should destroy relation" do
    assert_difference('Relation.count', -1) do
      delete :destroy, id: @relation
    end

    assert_redirected_to relations_path
  end
end
