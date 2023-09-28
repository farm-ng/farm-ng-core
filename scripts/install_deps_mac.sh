#!/bin/bash

set -x # echo on
set -e # exit on error

brew install --verbose \
    assimp \
    boost \
    catch2 \
    ccache \
    ceres-solver \
    cli11 \
    eigen \
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
    nlohmann-json \
    opencv \
    openexr \
    openssl \
    pre-commit \
    protobuf \
    tl-expected \
    zstd
