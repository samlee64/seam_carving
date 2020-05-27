#!/usr/bin/env bash

if [ $# != 1 ]; then
  echo "Missing migration name"
  echo "./scripts/create-migration.sh <migration name>"
  exit 1
else
  node_modules/db-migrate/bin/db-migrate create $1 --sql-file
fi

