#!/bin/bash
set -ex


ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." >/dev/null 2>&1 && pwd)"


RELEASE_SHA=`cat $ROOT_DIR/release_sha.txt`

echo "RELEASE_SHA: $RELEASE_SHA"

# If the needed RELEASE_SHA doesn't match what we have, download
if ! grep -q $RELEASE_SHA "$ROOT_DIR/venv/prefix/release_version.txt"; then
  mkdir -p "$ROOT_DIR/venv"
  $ROOT_DIR/scripts/download_release.sh
  rm -rf $ROOT_DIR/venv/prefix
  tar -xzf $ROOT_DIR/scripts/venv.tar.gz --strip-components=1 -C $ROOT_DIR/
  mv $ROOT_DIR/build_venv/venv/prefix/ $ROOT_DIR/venv/
  echo "$RELEASE_SHA" > $ROOT_DIR/venv/prefix/release_version.txt
  echo "[Updated binary dependencies. Done!]"
else
  echo "[Updated binary dependencies. Already up to date!]"
fi
