#!/bin/bash
set -e

# Usage:
#   ./test.sh [test_name]
#   Executes the given test. Tests are in the 'test' directory
#   If no test_name is provided all tests are executed.


execute_test() {
  echo Running c0
  ./c0 test/$1.c tmp.txt
  echo Running c1
  ./c1 tmp.txt tmp.s
  gcc -m32 tmp.s -o runtest

  echo
  ./runtest | tee result.txt
  echo

  diff  -sZ test/$1_expected.txt result.txt
}

make
if [[ "$1" == "" ]]; then
  for TEST_NAME in test/*.c; do
    TEST_NAME=$(basename $TEST_NAME)
    TEST_NAME=${TEST_NAME%.c}

    if execute_test $TEST_NAME > /dev/null; then
      echo $TEST_NAME: ok
    else
      echo $TEST_NAME: fail
    fi
  done

else
  execute_test $1
fi