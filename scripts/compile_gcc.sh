#!/bin/bash

set -x # echo on
set -e # exit on error

mkdir -p build
cd build
cmake -DBUILD_FARM_NG_PROTOS=On ..
make -j1
make test