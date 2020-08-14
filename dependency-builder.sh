#!/bin/bash
# RPEngine Dependecy resolver

mkdir -p tmp
mkdir -p includes/RPAudio

cd tmp
git clone --single-branch --branch dev https://github.com/AnzoDK/RPAudio.git
cd RPAudio
./BuildLib.sh
if [ $? -ne 0 ]
then
    echo "Could not build RPAudio!"
    exit 1
fi

mv -f RPAudio.so ../../includes/RPAudio/librpaudio.so
mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
cd ../..
rm -r -f tmp
