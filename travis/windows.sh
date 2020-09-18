#!/bin/bash
pwd
rm -rf RPAudio
./dependency-builder.sh --use-dev --Windows
make lib OS=Windows
if [ $? -ne 0 ]
then
    exit 1
fi
echo "Completed!"
