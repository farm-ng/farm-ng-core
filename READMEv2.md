# Overview

- Uses CMakePresets.json

- Uses find_package for third-party dependencies

- Supports 

- Developed in a devcontainer to avoid pollution by system-installed libraries

- Tested in a devcontainer

# TODO
- [] Custom registry (if necessary)
- [] Document vcpkg workflow
- [] CI tests of build
- [] CMakePresets.json for build configs
- [] Library features (enable testing, enable protos, etc)
- [] Audit / update macros
- [] Validate `install`
- [] Validate shared and static library support
- [] Binary caching
- [] Extract Sophus

# Build Instructions
./vcpkg/bootstrap-vcpkg.sh