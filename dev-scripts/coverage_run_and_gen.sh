#!/bin/bash

#setup
set -e
set -x

find ./ -iname "*.gc*" -delete
make clean
make -j
make test
lcov -o out.info -c -d .
genhtml -o html out.info
