#!/bin/bash -ex

# This script downloads the matching farm_ng_cmake Github release based on the
# current git SHA and inferred platform version. The release and platform can be
# overridden through the RELEASE_SHA and RELEASE_PLATFORM env variables.

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

# Choose release SHA based on git repo SHA
RELEASE_SHA="${RELEASE_SHA:-$(git rev-parse --short HEAD)}"

# Guess platform based on current platform
if [[ -z "${RELEASE_PLATFORM}" ]]; then
  if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    . /etc/lsb-release
    RELEASE_PLATFORM="${DISTRIB_ID,,}-${DISTRIB_RELEASE}"
  elif [[ "$OSTYPE" == "darwin"* ]]; then
    if [[ $(uname -m) == 'arm64' ]]; then
      RELEASE_PLATFORM="macos-arm64"
    else
      RELEASE_PLATFORM="macos-11"
    fi
  else
    echo "Couldn't guess Platform. Set RELEASE_PLATFORM environment variable."
    false
  fi
else
  RELEASE_PLATFORM="${RELEASE_PLATFORM}"
fi

# Download Release
VENV_TAR_URL=https://github.com/farm-ng/cmake/releases/download/v.$RELEASE_SHA/venv-$RELEASE_PLATFORM.tar.gz
curl -L $VENV_TAR_URL > venv.tar.gz
