#!/bin/sh
# bundle install
rm -f tmp/pids/server.pid
exec "$@"