#!/bin/bash

set -x # echo on
set -e # exit on error

mkdir -p build
cd build
CC=clang CXX=clang++ cmake -G Ninja -DBUILD_ASAN=On -DCMAKE_BUILD_TYPE=Debug -DBUILD_FARM_NG_PROTOS=On -DBUILD_SOPHUS_TESTS=On ..
ninja
ninja test
