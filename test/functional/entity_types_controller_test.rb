require 'test_helper'

class EntityTypesControllerTest < ActionController::TestCase
  setup do
    @entity_type = entity_types(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:entity_types)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create entity_type" do
    assert_difference('EntityType.count') do
      post :create, entity_type: { category: @entity_type.category, collection_id: @entity_type.collection_id, name: @entity_type.name, nomenclature: @entity_type.nomenclature, url: @entity_type.url }
    end

    assert_redirected_to entity_type_path(assigns(:entity_type))
  end

  test "should show entity_type" do
    get :show, id: @entity_type
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @entity_type
    assert_response :success
  end

  test "should update entity_type" do
    put :update, id: @entity_type, entity_type: { category: @entity_type.category, collection_id: @entity_type.collection_id, name: @entity_type.name, nomenclature: @entity_type.nomenclature, url: @entity_type.url }
    assert_redirected_to entity_type_path(assigns(:entity_type))
  end

  test "should destroy entity_type" do
    assert_difference('EntityType.count', -1) do
      delete :destroy, id: @entity_type
    end

    assert_redirected_to entity_types_path
  end
end
