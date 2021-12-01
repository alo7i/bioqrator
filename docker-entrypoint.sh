#!/bin/sh
# bundle install
# bundle exec rake assets:precompile

rm -f tmp/pids/server.pid
exec "$@"