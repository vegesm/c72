#!/bin/sh



# convert original asm listing and remove comments
cat $1  |  sed -r 's|/(.*)|/\*\1 */|g'  | \
	# convert the lookup table at the start to use .int directives
	sed -r 's/([0-9]+)\.;[[:blank:]]+([A-Za-z0-9]+)/.int \1, \2/g' | \
	# replace .even directive with .balign, replace first row of the lookup table
	sed 's/\.even/.balign 4/g' | sed -r 's/_(eff|reg|sp|cc)tab=\.;[[:blank:]]*\.\+2/_\1tab=.;.int .+4/g'  | \
	# replace single `0` characters with .int directive
	sed -r 's/^([[:blank:]]+)0$/\1.int 0/g'

# sed -r 's/_(eff|reg|sp|cc)tab=\.;[[:blank:]]*\.\+2/_\1tab=.int .,.+4/g'