#!/bin/bash
set -e

rm build -rf
mkdir -p build && cd build && CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release ..
cd ..

# run clang tidy
run-clang-tidy  -quiet -p build/ farm_ng_core/.*\.cpp -fix
