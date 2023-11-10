#!/bin/bash -ex
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR
python3 -m venv venv
source venv/bin/activate
pip install -U pip
pip install -U wheel
pip install -U cmake ninja

cmake -B build.venv \
    -DFARM_NG_DEV_BUILD=On \
    -Dfarm_ng_INSTALL_PREFIX=$DIR/venv/prefix \
    -G Ninja \
    $DIR/.

ninja -C build.venv -j4
