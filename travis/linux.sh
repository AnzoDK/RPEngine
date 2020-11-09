#!/bin/bash
pwd
./dependency-builder.sh --use-dev
make USE_LOCAL=1
./rpengine
make clean
make lib OS=Linux USE_LOCAL=1
