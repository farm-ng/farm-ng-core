# Overview

- Uses CMakePresets.json

- Separate package description from development environment

- Uses find_package for third-party dependencies

- Uses vcpkg as a package manager (not the only option)
- Ideally addressing potential ODR violations

- Developed in a devcontainer to avoid pollution by system-installed libraries
- Uses cmake modules for more declarative cmake files
- Includes testing?

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