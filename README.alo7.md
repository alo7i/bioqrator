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

## resources
- https://github.com/dongseop/bioqrator