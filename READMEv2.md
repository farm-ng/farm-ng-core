# Overview

- Uses CMakePresets.json to define common build configurations as presets
 - Includes presets for clang/gcc, debug/release, clang-tidy asan, tsan, msan
 - Persistent build directories per preset
 - Great Vscode integration 
 - Developer can add CMakeUserPresets.json for additional configurations

- Separate package definition from development environment 
 - Modern cmake best practices encourage us to separate build/packaging requirements (used by downstream consumers)
   from the development environment.
 - For example:
   - Our CMake should not specify where to find dependencies, only that they are found by find_package
   - Our CMake should not specify build flags, only target requirements (e.g. target_compile_features) 

- Leverage vcpkg as a package manager
 - This setup does not require use of a package manager, but recommends it and uses it in CI
 - Vcpkg has been easy to setup and the semantics feel minimal and complete
 - To me, use of a package manager feels like the only way to address potential ODR violations. 
   You need a centralized place to reason about package versioning compatibility across the entire dependency tree.
 - Eliminates the need for our custom C++ "virtualenv" setup

- Easily switch between development / released / system versions of packages
 - Switching to a development version relies on a few lines of code in `cmake/farm_ng_dev_packages.cmake` and one-line edits to `CMakePresets.json` and `vcpkg.json`
   This seems to be working so far. I also played with CMake's "Dependency Providers" feature which allows more customization if necessary.
 - Switching to a system version requires a one-line edit to `vcpkg.json`, and then find_package() should resolve to a system installed version.

- Move towards Sophus as separate project
 - For simplicity it is still vendored in here, but can easily be made a submodule or pulled in with vcpkg

- Developed with devcontainer.json
  - Dockerized development environment is optional but not required
  - Better CI reproducibility

- Better cross-platform support
  - OSX should work, clear path to Windows
  - Clear path to support cross-compilation
  - Build configurations extracted into CMakePresets.json
  - Cross-platform package manager (vcpkg)

- Begins to explore refactoring/cleanup of farm_ng module implementations in `cmake/`

# Instructions

```
./vcpkg/bootstrap-vcpkg.sh
```

# FAQ

Q: How does this work for repositories downstream of farm-ng-core? What about consuming closed-source packages?
A: Largely the same. It will help if we define a vcpkg "port" for each farm-ng C++ package ([an example](https://github.com/northwindtraders/vcpkg-registry/blob/main/ports/beicode/portfile.cmake)). We can then host these ports in a farm-ng vcpkg registry, which is just a (possibly private) Github repository ([an example](https://github.com/northwindtraders/vcpkg-registry/tree/main)). I have not tried this yet.

Q: Is building dependencies from source too slow?
A: So far, in this repository, no. I know Ceres and OpenCV could be slower to build. And we won't want to rebuild on every run in CI. Vcpkg's support for [binary caching](https://learn.microsoft.com/en-us/vcpkg/users/binarycaching) is the next obvious optimization.

# TODO
- Define and (re-)implement package options to enable/disable testing, protos, etc.
- Finish refactor of farm_ng macros in `cmake/`
- Validate updated package `install` logic
- Validate advanced packaging features: shared and static library support, etc.  
- Add CI, including an integration test of a simple "consuming" project
