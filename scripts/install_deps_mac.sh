#!/bin/bash

set -x # echo on
set -e # exit on error

brew install --verbose \
    ccache \
    glog \
    ninja


# brew install --verbose \
#     assimp \
#     catch2 \
#     ccache \
#     ffmpeg \
#     glew \
#     glog \
#     libjpeg \
#     libpng \
#     libtiff \
#     lz4 \
#     opencv \
#     openexr \
#     openssl \
#     pre-commit \
#     zstd
