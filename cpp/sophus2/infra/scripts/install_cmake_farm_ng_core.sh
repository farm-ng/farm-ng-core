#!/bin/bash

set -x # echo on
set -e # exit on error

cmake --version

rm -rf cmake-build
rm -rf farm-ng-core-build

cd ../ci-submodules

mkdir cmake-build
cd cmake-build
cmake -G Ninja ../cmake
ninja
sudo ninja install
cd ..

mkdir farm-ng-core-build
cd farm-ng-core-build
cmake -G Ninja -DBUILD_FARM_NG_PROTOS=On ../farm-ng-core
ninja
sudo ninja install
cd ..

cd ../scripts
