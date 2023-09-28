#!/bin/bash

set -x # echo on
set -e # exit on error

brew install --verbose \
    assimp \
    boost \
    ccache \
    catch2 \
    ceres-solver \
    cli11 \
    eigen \
    expected \
    ffmpeg \
    fmt \
    glog \
    glew \
    grpc \
    libjpeg \
    libpng \
    libtiff \
    lz4 \
    ninja \
    nlohmann_json \
    opencv \
    openexr \
    openssl \
    pre-commit \
    protobuf \
    zstd
