#!/bin/bash -ex

PYTHON_TESTS_DIR=./py/tests
PYTHON_ASYNCIO_TESTS_DIR=$PYTHON_TESTS_DIR/_asyncio

EXIT_STATUS=0
# if flag --asyncio is passed, run only asyncio tests
# NOTE: we need to run asyncio tests separately because they require a different event loop
if [[ "$*" == *"--asyncio"* ]]; then
    # iterate over all files in the directory
    for file in $PYTHON_ASYNCIO_TESTS_DIR/*.py; do
        # run the test
        pytest -v $file || EXIT_STATUS=$?
    done

# otherwise, run all tests except asyncio tests
else
    pytest -v $PYTHON_TESTS_DIR --ignore $PYTHON_ASYNCIO_TESTS_DIR || EXIT_STATUS=$?
fi
exit $EXIT_STATUS
