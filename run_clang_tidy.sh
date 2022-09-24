#!/bin/bash
set -e

rm build -rf
mkdir -p build && cd build && CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_FARM_NG_PROTOS=OFF ..
cd ..

# run clang tidy
run-clang-tidy -quiet -p build/ farm_ng/core/.*\.cpp -fix -style none
