#!/bin/bash -ex
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

PYTHON_TESTS_DIR=$DIR/py/tests
PYTHON_ASYNCIO_TESTS_DIR=$PYTHON_TESTS_DIR/_asyncio

EXIT_STATUS=0
# if flag --asyncio is passed, run only asyncio tests
# NOTE: we need to run asyncio tests separately because they require a different event loop
if [[ "$*" == *"--asyncio"* ]]; then
    # iterate over all files in the directory
    for file in $PYTHON_ASYNCIO_TESTS_DIR/*.py; do
        # run the test
        pytest -vv $file || EXIT_STATUS=$?
    done

# otherwise, run all tests except asyncio tests
else
    pytest -vv $PYTHON_TESTS_DIR --ignore $PYTHON_ASYNCIO_TESTS_DIR || EXIT_STATUS=$?
fi
exit $EXIT_STATUS
