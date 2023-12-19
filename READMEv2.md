# Overview

- Uses CMakePresets.json to define common build configurations as presets

  - Includes presets for clang/gcc, debug/release, clang-tidy asan, tsan, msan
  - Persistent build directories per preset
  - Great Vscode integration
  - Developer can add CMakeUserPresets.json for additional configurations

- Separate package definition from development environment

  - Modern cmake best practices encourage us to separate build/packaging requirements (used by downstream consumers)
    from the development environment (used by developers)
  - For example:

    - Our CMakeLists.txt should not specify where to find dependencies, only that they are found by `find_package`
    - Our CMakeLists.txt should not specify build flags, only target requirements (e.g. `target_compile_features`)

- Leverage vcpkg as a package manager

  - This setup does not require use of a package manager, but recommends it and uses it in CI
  - Vcpkg has been easy to setup and the semantics feel minimal and complete
  - To me, use of a package manager feels like the only way to address potential ODR violations.
    You need a centralized place to reason about package versioning compatibility across the entire dependency tree.
  - Eliminates the need for our custom C++ "virtualenv" setup
  - Open-source contributors contribute patches that improve packaging of popular libraries (e.g. support for static linking)

- Easily switch between development / released / system versions of packages

  - Switching to a development version relies on a few lines of code in `cmake/farm_ng_dev_packages.cmake` and one-line edits to `CMakePresets.json` and `vcpkg.json`
    This seems to be working so far -- it is demonstrated here with sophus, Eigen, and CLI11. I also played with CMake's "Dependency Providers" feature which allows more customization if necessary.
  - Switching to a system version requires a one-line edit to `vcpkg.json`, and then find_package() should resolve to a system installed version.

- Move towards Sophus as separate project

  - For simplicity it is still vendored in here, but can easily be made a submodule or pulled in with vcpkg

- Optional devcontainer.json

  - Dockerized development environment is optional but not required
  - Better reproducibility for CI
  - One-click Github Codespaces integration

- Better cross-platform support

  - OSX should work, clear path to Windows
  - Clear path to support cross-compilation

- Begins to explore refactoring/cleanup of farm_ng module macros in `cmake/`
  - There are some new Cmake APIs and recommended patterns we can take advantage of

# Instructions

```bash
# Open a terminal **without** the farm-ng venv activated
# Should print 'OK':
printenv CMAKE_PREFIX_PATH || echo "OK"

# Ensure you have a reasonably up-to-date cmake (tested with 3.25) and ninja (tested with 1.10)
cmake --version
ninja --version

# Update submodules
git submodule update --init --recursive

# Bootstrap vcpkg
./vcpkg/bootstrap-vcpkg.sh

# Configure
# Note: Builds dependencies from source on first invocation, may take 5-10mins
cmake --preset clang-debug

# Build
cmake --build --preset clang-debug

# Test (one test fails on my machine)
ctest --preset clang-debug

# Install
mkdir /tmp/farm-ng-prefix
cmake --build --preset clang-debug --target install -DCMAKE_PREFIX_PATH=/tmp/farm-ng-prefix

# Try the same workflows in Vscode
# Select a configure preset such as "Clang RelWithDebInfo", then build and run as usual
code farm_ng_core.code-workspace

# Optionally, you can try the devcontainer workflow in Vscode by clicking "Reopen in Container" when prompted
# - Note, this will not re-use the same vcpkg cache, so the first build will take more time
# - Note, if you select a configure preset in the devcontainer that you've already built on your host machine you will need to "CMake: Delete Cache and Reconfigure"
```

# FAQ

**Q: How does this work for repositories downstream of farm-ng-core? What about consuming closed-source packages?**

A: Largely the same. It will help if we define a vcpkg "port" for each farm-ng C++ package ([an example](https://github.com/northwindtraders/vcpkg-registry/blob/main/ports/beicode/portfile.cmake)). We can then host these ports in a farm-ng vcpkg registry, which is just a (optionally private) Github repository ([an example](https://github.com/northwindtraders/vcpkg-registry/tree/main)). I have not tried this yet.

**Q: Is building dependencies from source too slow?**

A: So far, in this repository, no. But it does not rely on Ceres and OpenCV. And we won't want to rebuild on every run in CI. Vcpkg's support for [binary caching](https://learn.microsoft.com/en-us/vcpkg/users/binarycaching) is the next obvious optimization.

# TODO

- Define and (re-)implement package options to enable/disable testing, protos, etc.
- Finish refactor of farm_ng macros in `cmake/`
- Update and validate updated package `install` logic
- Specify desired third-party dependency versions in vcpkg.json
- Validate advanced packaging features: shared and static library support, etc.
- Add CI, including an integration test of a simple "consuming" project
- Add CMake linting if valuable
