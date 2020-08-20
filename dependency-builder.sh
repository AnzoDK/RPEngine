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
		./BuildLibWindows.sh
        if [ $? -eq 0 ]
        then
            exit 1
        fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f RPAudio.dll ../../includes/RPAudio/librpaudio.dll
	else
		./BuildLib.sh
		if [ $? -eq 0 ]
        then
            exit 1
        fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f RPAudio.so ../../includes/RPAudio/librpaudio.so
	fi
else
	if [ "$1" == "--Windows" ]
	then
		./BuildLibWindows.sh
        if [ $? -eq 0 ]
        then
            exit 1
        fi
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
		mv -f RPAudio.dll ../../includes/RPAudio/librpaudio.dll
	else
		./BuildLib.sh
        if [ $? -eq 0 ]
        then
            exit 1
        fi
		mv -f RPAudio.so ../../includes/RPAudio/librpaudio.so
		mv -f includes/rpaudio.h ../../includes/RPAudio/rpaudio.h
		mv -f includes/commontools.h ../../includes/RPAudio/commontools.h
	fi
fi

cd ../..
rm -r -f tmp
