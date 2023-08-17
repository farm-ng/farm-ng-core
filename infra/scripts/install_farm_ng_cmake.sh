#!/bin/bash

set -x # echo on
set -e # exit on error

cmake --version
source ../venv/bin/activate
cd ../ci-submodules
rm -rf cmake-build
mkdir cmake-build
cd cmake-build
cmake -G Ninja ../cmake -DCMAKE_INSTALL_PREFIX=${CMAKE_PREFIX_PATH}
ninja
ninja install
cd ..

cd ../scripts
