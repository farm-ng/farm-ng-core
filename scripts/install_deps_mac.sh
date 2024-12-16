#!/bin/bash

set -x # echo on
set -e # exit on error

brew install --verbose \
    assimp \
    catch2 \
    ccache \
    ffmpeg \
    glew \
    libjpeg \
    libpng \
    libtiff \
    lz4 \
    ninja \
    opencv \
    openexr \
    openssl \
    pre-commit \
    zstd
