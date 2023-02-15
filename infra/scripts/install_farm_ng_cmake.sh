#!/bin/bash

set -x # echo on
set -e # exit on error

cmake --version

rm -rf farm-ng-cmake-build

cd ../ci-submodules

mkdir farm-ng-cmake-build
cd farm-ng-cmake-build
cmake -G Ninja ../farm-ng-cmake
ninja
sudo ninja install
cd ..

cd ../scripts
