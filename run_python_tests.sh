#!/bin/bash

PYTHON_TESTS_DIR=./py/tests
PYTHON_ASYNCIO_TESTS_DIR=$PYTHON_TESTS_DIR/_asyncio

# if flag --asyncio is passed, run only asyncio tests
# NOTE: we need to run asyncio tests separately because they require a different event loop
if [[ "$*" == *"--asyncio"* ]]; then
    # iterate over all files in the directory
    for file in $PYTHON_ASYNCIO_TESTS_DIR/*.py; do
        # run the test
        pytest -v $file
    done
    exit 0

# otherwise, run all tests except asyncio tests
else
    pytest -v $PYTHON_TESTS_DIR --ignore $PYTHON_ASYNCIO_TESTS_DIR
    exit 0
fi
