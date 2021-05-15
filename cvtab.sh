#!/bin/bash

UNDERSCORE=_
if [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
	UNDERSCORE=
fi

# remove leading underscore on linux
#cat $1  |  ./cvopt |	sed -r 's/_(eff|reg|sp|cc)tab/'$UNDERSCORE'\1tab/'
# remove multiline comments by convert '*/' to @ and then doing a lazy match

#cat $1  |   sed -r 's|\*/|}|g' | sed -r 's|/\*|{|g'
cat $1  |   sed -r 's|\*/|@|g' | sed -r ':a;N;$!ba;s|/\*[^@]+@||g' | \
  cmake-build-debug-mingw/cvopt |	sed -r 's/_(eff|reg|sp|cc)tab/'$UNDERSCORE'\1tab/'