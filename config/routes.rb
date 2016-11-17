Curator::Application.routes.draw do

  match 'uniprot/:id' => 'api#fetch_uniprot'
   
  get "api/uniprot"
  get "api/entrez"
  match "api/update/:pmid" => 'api#update_document'
  get "documents/fetch"
  post "documents/check"

  resources :info_nodes


  resources :nodes do
    member do
      get 'info'
    end
  end

  resources :nodes do
    member do 
      get 'merge'
      get 'remove'
      get 'add_all'
    end
  end

  resources :collections do
    collection do 
      post 'upload'
    end

    resources :labels
    resources :recommenders
    resources :save_queries do
      member do
        get 'refresh'
      end
    end

    member do 
      get 'share'
      post 'add_share'
      post 'remove_share'
      post 'upload'
      post 'mark_negative'
      post 'unmark_negative'
      get 'remove_documents'
      get 'remove_annotations'
    end
    
    resources :annotations
    resources :relations
    resources :documents do 
      collection do 
        get 'query'
      end
      member do
        post 'new'
        post 'done'
        get 'change'
        get 'load_pie'
        post 'import_pie'
        post 'refetch'
      end
      resources :annotations
      resources :relations
    end
    resources :entity_types
  end
  

  get "home/index"
  get "home/about"
  
  devise_for :users

  scope "/admin" do
    root :to => 'admin#index' 
    resources :users
    match 'refresh' => 'admin#refresh'
  end

  # The priority is based upon order of creation:
  # first created -> highest priority.

  # Sample of regular route:
  #   match 'products/:id' => 'catalog#view'
  # Keep in mind you can assign values other than :controller and :action

  # Sample of named route:
  #   match 'products/:id/purchase' => 'catalog#purchase', :as => :purchase
  # This route can be invoked with purchase_url(:id => product.id)

  # Sample resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products

  # Sample resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Sample resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Sample resource route with more complex sub-resources
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', :on => :collection
  #     end
  #   end

  # Sample resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end

  # You can have the root of your site routed with "root"
  # just remember to delete public/index.html.
  root :to => 'home#index'

  # See how all your routes lay out with "rake routes"

  # This is a legacy wild controller route that's not recommended for RESTful applications.
  # Note: This route will make all actions in every controller accessible via GET requests.
  # match ':controller(/:action(/:id))(.:format)'
end
