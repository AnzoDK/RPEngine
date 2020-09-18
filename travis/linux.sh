#!/bin/bash
pwd
./dependency-builder.sh --use-dev
make
./rpengine
make clean
make lib OS=Linux
