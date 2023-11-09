import os
import sys
from pathlib import Path

from farm_ng.package.commands import (
    BuildProtosDevelop,
    BuildProtosEggInfo,
    BuildProtosInstall,
    CleanFilesCommand,
)
from pybind11.setup_helpers import ParallelCompile, Pybind11Extension, build_ext
from setuptools import setup

__version__ = "2.0.0"


platform_cxx_flags = []

if sys.platform.startswith("darwin"):
    print("Running on macOS")
    platform_cxx_flags = [
        "-std=c++20",
        "-mmacosx-version-min=11.0",
        "-D_LIBCPP_DISABLE_DEPRECATION_WARNINGS=1",
    ]
    os.environ["MACOSX_DEPLOYMENT_TARGET"] = "11.0"

elif sys.platform.startswith("linux"):
    print("Running on Linux")
    platform_cxx_flags = [
        "-std=gnu++17",
        "-fconcepts",
    ]
else:
    print("Running on another operating system")

PROTO_ROOT: str = "protos"
PACKAGE_ROOT: str = "py"


BuildProtosDevelop.user_options.append(("proto-root=", None, PROTO_ROOT))
BuildProtosDevelop.user_options.append(("package-root=", None, PACKAGE_ROOT))

BuildProtosInstall.user_options.append(("proto-root=", None, PROTO_ROOT))
BuildProtosInstall.user_options.append(("package-root=", None, PACKAGE_ROOT))

BuildProtosEggInfo.user_options.append(("proto-root=", None, PROTO_ROOT))
BuildProtosEggInfo.user_options.append(("package-root=", None, PACKAGE_ROOT))

CleanFilesCommand.user_options.append(("package-root=", None, PACKAGE_ROOT))


# Optional multithreaded build
ParallelCompile("CMAKE_BUILD_PARALLEL_LEVEL", default=4).install()

source_path = Path(__file__).resolve()
source_dir = source_path.parent


# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)


ext_modules = [
    Pybind11Extension(
        "farm_ng_core_pybind",
        [
            # farm-ng-control
            "py/pybind/farm_ng_core_pybind.cpp",
            "py/pybind/linalg_pybind.cpp",
            "py/pybind/lie_pybind.cpp",
            # fmt
            "cpp/thirdparty/fmt/src/format.cc",
            "cpp/thirdparty/fmt/src/os.cc",
            # farm-ng-core
            "cpp/farm_ng/core/logging/backtrace.cpp",
            "cpp/farm_ng/core/logging/expected.cpp",
            "cpp/farm_ng/core/logging/format.cpp",
            "cpp/farm_ng/core/logging/logger.cpp",
        ],
        define_macros=[("VERSION_INFO", __version__)],
        extra_compile_args=[
            *platform_cxx_flags,
            "-Wall",
            "-Wextra",
            "-Werror",
            "-Wno-unknown-warning-option",
            "-Wno-unused-parameter",
            "-Wno-missing-field-initializers",
            "-Wno-unused-but-set-variable",
            "-Wno-unused-variable",
            "-Wno-unused-function",
            "-Wno-maybe-uninitialized",
        ],
        include_dirs=[
            source_dir / "cpp",
            source_dir / "cpp/thirdparty/expected/include",
            source_dir / "cpp/thirdparty/farm_pp/include",
            source_dir / "cpp/thirdparty/fmt/include",
            source_dir / "cpp/thirdparty/eigen",
        ],
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={
        "install": BuildProtosInstall,
        "develop": BuildProtosDevelop,
        "egg_info": BuildProtosEggInfo,
        "clean": CleanFilesCommand,
        "build_ext": build_ext,
    },
)
