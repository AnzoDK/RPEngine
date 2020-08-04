#!/bin/bash
# RPEngine Dependecy resolver

mkdir -p tmp
mkdir -p includes/RPAudio
mkdir -p includes/oggvorbis
mkdir -p includes/vorbisfile
mkdir -p includes/libopenal

cd tmp
git clone --single-branch --branch dev https://github.com/AnzoDK/RPAudio.git
cd RPAudio
./BuildLib.sh
if [ $? -ne 0 ]
then
    echo "Could not build RPAudio!"
    exit 1
fi

mv -f RPAudio.a ../../includes/RPAudio/libRPAudio.a
mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
mv -f includes/oggvorbis/*.h ../../includes/oggvorbis/
mv -f includes/vorbisfile/*.h ../../includes/vorbisfile/
mv -f includes/libopenal/*.h ../../includes/libopenal/
cd ../..
rm -r -f tmp
