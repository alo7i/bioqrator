require 'test_helper'

class NodesControllerTest < ActionController::TestCase
  setup do
    @node = nodes(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:nodes)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create node" do
    assert_difference('Node.count') do
      post :create, node: { document_id: @node.document_id, entity_key: @node.entity_key, entity_type_id: @node.entity_type_id, node_id: @node.node_id, node_text: @node.node_text, node_type: @node.node_type, passage_id: @node.passage_id }
    end

    assert_redirected_to node_path(assigns(:node))
  end

  test "should show node" do
    get :show, id: @node
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @node
    assert_response :success
  end

  test "should update node" do
    put :update, id: @node, node: { document_id: @node.document_id, entity_key: @node.entity_key, entity_type_id: @node.entity_type_id, node_id: @node.node_id, node_text: @node.node_text, node_type: @node.node_type, passage_id: @node.passage_id }
    assert_redirected_to node_path(assigns(:node))
  end

  test "should destroy node" do
    assert_difference('Node.count', -1) do
      delete :destroy, id: @node
    end

    assert_redirected_to nodes_path
  end
end
