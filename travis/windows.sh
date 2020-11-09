#!/bin/bash
pwd
rm -rf RPAudio
./dependency-builder.sh --use-dev --Windows
make lib OS=Windows USE_LOCAL=1
if [ $? -ne 0 ]
then
    exit 1
fi
echo "Completed!"
