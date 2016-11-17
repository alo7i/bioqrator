require 'test_helper'

class InfoNodesControllerTest < ActionController::TestCase
  setup do
    @info_node = info_nodes(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:info_nodes)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create info_node" do
    assert_difference('InfoNode.count') do
      post :create, info_node: { collection_id: @info_node.collection_id, document_id: @info_node.document_id, key: @info_node.key, node_id: @info_node.node_id, node_type: @info_node.node_type, passage_id: @info_node.passage_id, title: @info_node.title }
    end

    assert_redirected_to info_node_path(assigns(:info_node))
  end

  test "should show info_node" do
    get :show, id: @info_node
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @info_node
    assert_response :success
  end

  test "should update info_node" do
    put :update, id: @info_node, info_node: { collection_id: @info_node.collection_id, document_id: @info_node.document_id, key: @info_node.key, node_id: @info_node.node_id, node_type: @info_node.node_type, passage_id: @info_node.passage_id, title: @info_node.title }
    assert_redirected_to info_node_path(assigns(:info_node))
  end

  test "should destroy info_node" do
    assert_difference('InfoNode.count', -1) do
      delete :destroy, id: @info_node
    end

    assert_redirected_to info_nodes_path
  end
end
