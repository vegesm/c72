#!/bin/bash
set -e

cd cmake-build-debug-mingw
mingw32-make c0
mingw32-make c1

echo Running c0
./c0 ../test/$1.c tmp.txt
echo Running c1
./c1 tmp.txt tmp.s
gcc tmp.s -o test.exe

echo
./test.exe | tee result.txt
echo

diff  -s ../test/$1_expected.txt result.txt