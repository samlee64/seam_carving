#!/usr/bin/env bash

nodemon --exec "build-if-changed" lib/main.js
