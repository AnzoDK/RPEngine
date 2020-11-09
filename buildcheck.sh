#!/bin/bash
LOCAL=0
echo "#include <RPAudio/RPAudio.h>\n int main(){return 0;}" >> check.cpp
g++ check.cpp -o test.o >> /dev/null
if [ $? -ne 0 ]
then
  echo "RPAudio is not correctly installed - building against a local copy instead!"
  $LOCAL = 1
else
  rm test.o
fi
exit $LOCAL
