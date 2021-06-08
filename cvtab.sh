#!/bin/bash

UNDERSCORE=_
if [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
	UNDERSCORE=
fi

# remove multiline comments by converting '*/' to @ and then doing a lazy match
# remove leading underscore on Linux
cat $1  |   sed -r 's|\*/|@|g' | sed -r ':a;N;$!ba;s|/\*[^@]+@||g' | \
  ./cvopt |	sed -r 's/_(eff|reg|sp|cc)tab/'$UNDERSCORE'\1tab/'