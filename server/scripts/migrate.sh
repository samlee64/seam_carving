#!/usr/bin/env bash

if [ "$1" != "up" ] &&  [ "$1" != "down" ]; then
  echo "Specify \"up\" or \"down\" "
else
  if [ "$#" -eq 2 ]; then
    echo Migrating for $2 env
    node_modules/db-migrate/bin/db-migrate $1 --config database.json -e $2
  else
    node_modules/db-migrate/bin/db-migrate $1 --config database.json -e dev
  fi
fi
