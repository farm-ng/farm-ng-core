#!/bin/bash
set -e

rm -rf clang-tidy-build
mkdir clang-tidy-build
cd clang-tidy-build
CC=clang CXX=clang++ cmake ..
make -j2
cd ..

# run clang tidy
run-clang-tidy-12 -quiet -p clang-tidy-build cpp/farm/.*\.cpp -fix -style none
