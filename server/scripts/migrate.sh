#!/usr/bin/env bash

if [ "$1" != "up" ] &&  [ "$1" != "down" ]; then
  echo "Specify \"up\" or \"down\" "
else
  node_modules/db-migrate/bin/db-migrate $1 --config database.json -e dev
fi
