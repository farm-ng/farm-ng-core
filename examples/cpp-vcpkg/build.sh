#!/bin/bash
set -e

# Execute commands from this script's directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

# Bootstrap vcpkg
git submodule update --init --recursive
./vcpkg/bootstrap-vcpkg.sh -disableMetrics

# Use latest farm-ng-core vcpkg ports as a local overlay
# https://learn.microsoft.com/en-us/vcpkg/concepts/overlay-ports
rsync -a --delete $DIR/../../cmake/vcpkg/ports/ ./overlay-ports/

# In the overlay, use the current git sha as the farm-ng-core version
portfile=$DIR/overlay-ports/farm-ng-core/portfile.cmake
git_sha=`git rev-parse HEAD`
git_sha512=`curl -sL https://github.com/farm-ng/farm-ng-core/archive/$git_sha.tar.gz | shasum -a 512 | head -c 128`
sed -i "s/ REF.*$/ REF $git_sha # Auto-generated/" $portfile
sed -i "s/ SHA512.*$/ SHA512 $git_sha512 # Auto-generated/" $portfile

# Build
cmake --preset clang-debug
cmake --build --preset clang-debug

# Run
./out/build/clang-debug/HelloWorld
