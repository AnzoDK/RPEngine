#!/bin/bash
# RPEngine Dependecy resolver
git --version &> /dev/null
if [ $? -eq 0 ]
then
    echo "Git installed - YES"
else
    echo "Git Installed - NO"
    echo "Git is needed for this script to work!"
    exit 1
fi
mkdir -p tmp
mkdir -p includes/RPAudio
cd tmp
if [ "$1" == "--use-dev" ]
then
    echo "Using dev branch"
    git clone --single-branch --branch dev https://github.com/AnzoDK/RPAudio.git
else
    echo "Using master branch"
    git clone https://github.com/AnzoDK/RPAudio.git
fi
cd RPAudio
if [ $? -ne 0 ]
then
    echo "Could not build RPAudio!"
    exit 1
fi
if [ "$1" == "--use-dev" ]
then
	if [ "$2" == "--Windows" ]
	then
		if [ "$3" == "--optimize" ]
		then
			./BuildLibWindows.sh --optimize
		else
			/BuildLibWindows.sh
		fi
        if [ $? -ne 0 ]
        then
            cd ../..
            rm -r -f tmp
	    echo "FATAL: RPAudio failed to build!"
            exit 1
        fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f rpaudio.dll ../../includes/RPAudio/librpaudio.dll
		mv -f librpaudio.a ../../includes/RPAudio/librpaudio.a
	else
		if [ "$2" == "--optimize" ]
		then
			./BuildLib.sh --optimize
		else
			./BuildLib.sh
		fi
		if [ $? -ne 0 ]
        	then
                cd ../..
	    		rm -r -f tmp
	    		exit 1
        	fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f rpaudio.so ../../includes/RPAudio/librpaudio.so
	fi
else
	if [ "$1" == "--Windows" ]
	then
		if [ "$2" == "--optimize" ]
		then
			./BuildLibWindows.sh --optimize
		else
			./BuildLibWindows.sh
		fi
        	if [ $? -ne 0 ]
        	then
            		cd ../..
	    		rm -r -f tmp
            		exit 1
       		fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f rpaudio.dll ../../includes/RPAudio/librpaudio.dll
		mv -f librpaudio.a ../../includes/RPAudio/librpaudio.a
	else
		if [ "$1" == "--optimize" ]
		then
			./BuildLib.sh --optimize
		else
			./BuildLib.sh
		fi
        if [ $? -ne 0 ]
        then
            cd ../..
	    rm -r -f tmp
            exit 1
        fi
		mv -f rpaudio.so ../../includes/RPAudio/librpaudio.so
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
	fi
fi
pwd
mkdir -p ../../includes/RPAudio/oggvorbis/
mkdir -p ../../includes/RPAudio/vorbis/
mkdir -p ../../includes/RPAudio/libopenal/
mkdir -p ../../includes/RPAudio/vorbisfile/

cp -f includes/oggvorbis/* ../../includes/RPAudio/oggvorbis/
cp -f includes/vorbis/* ../../includes/RPAudio/vorbis/
cp -f includes/libopenal/* ../../includes/RPAudio/libopenal/
cp -f includes/vorbisfile/* ../../includes/RPAudio/vorbisfile/
cd ../..
rm -r -f tmp
echo "Complete"
