#!/bin/bash
set -ex


ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." >/dev/null 2>&1 && pwd)"


RELEASE_SHA=f9f3bb6

# If the needed RELEASE_SHA doesn't match what we have, download
if ! grep -q $RELEASE_SHA "$ROOT_DIR/venv/prefix/release_version.txt"; then
  mkdir -p "$ROOT_DIR/venv"
  $ROOT_DIR/../cmake/scripts/download_release.sh
  rm -rf $ROOT_DIR/venv/prefix
  tar -xzf $ROOT_DIR/../cmake/scripts/venv.tar.gz --strip-components=1 -C $ROOT_DIR/
  echo "$RELEASE_SHA" > $ROOT_DIR/venv/prefix/release_version.txt
  echo "[Updated binary dependencies. Done!]"
else
  echo "[Updated binary dependencies. Already up to date!]"
fi
