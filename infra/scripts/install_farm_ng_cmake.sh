#!/bin/bash

set -x # echo on
set -e # exit on error

cmake --version
source ../venv/bin/activate
cd ../ci-submodules
rm -rf farm-ng-cmake-build
mkdir farm-ng-cmake-build
cd farm-ng-cmake-build
cmake -G Ninja ../farm-ng-cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
ninja
ninja install
cd ..

cd ../scripts
