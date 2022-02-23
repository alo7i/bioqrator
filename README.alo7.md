# Bioqrator
> A web-based interactive biomedical literature curating system.

## preview
- http://39.96.160.44/

## get-started
```shell
git clone git@github.com:alo7i/bioqrator.git
docker-compose up
```

## nginx
> /etc/nginx/sites-available/defatult
```conf
server {
  listen 80 default_server;
  listen [::]:80 default_server;
  index  index.html index.htm;

  location / {
    proxy_pass  http://127.0.0.1:3000; # 转发规则
    proxy_set_header Host $proxy_host; # 修改转发请求头，让8080端口的应用可以受到真实的请求
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
  }
}
```

## error
- 启动的时候有可能报错，问题出在 public/assets 目录无法编译通过
- 可以找个现成的(aric.zheng) assets 目录，然后放到 public/assets 下面。

~~~
ActionView::Template::Error (application.js isn't precompiled):
    9: <!-- Javascripts
    10:     ================================================== -->
    11:     <!-- Placed at the end of the document so the pages load faster -->
    12:     <%= javascript_include_tag "application" %>
    13: 
    14:     <!-- Le HTML5 shim, for IE6-8 support of HTML elements -->
    15:     <!--[if lt IE 9]>
  app/views/layouts/application.html.erb:12:in `_app_views_layouts_application_html_erb__765559360058906216_34702260'
~~~

## 正常的解决办法
- docker-entrypoint.sh 里打开这两句的注释
```shell
# bundle install
# bundle exec rake assets:precompile
```

## resources
- https://github.com/dongseop/bioqrator