#!/bin/bash

set -x # echo on
set -e # exit on error

brew install --verbose \
    assimp \
    catch2 \
    ccache \
    ffmpeg \
    fmt \
    glew \
    glog \
    grpc \
    libjpeg \
    libpng \
    libtiff \
    lz4 \
    ninja \
    opencv \
    openexr \
    openssl \
    pre-commit \
    protobuf \
    zstd
