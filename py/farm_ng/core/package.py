import os
import shutil
import sys
from pathlib import Path

from setuptools import Command
from setuptools.command.develop import develop
from setuptools.command.egg_info import egg_info
from setuptools.command.install import install


def build_package_protos(package_root, proto_includes=[]):
    """
    includes: additional include paths
    """
    from grpc_tools import protoc
    import pkg_resources

    proto_files = []
    inclusion_root = os.path.abspath(package_root)
    for root, _, files in os.walk(inclusion_root):
        for filename in files:
            if filename.endswith(".proto"):
                proto_files.append(os.path.abspath(os.path.join(root, filename)))

    well_known_protos_include = pkg_resources.resource_filename("grpc_tools", "_proto")
    includes = []

    for x in sys.path:
        if os.path.isdir(x):
            includes.append("--proto_path={}".format(x))

    for x in proto_includes:
        assert os.path.isdir(x)
        includes.append("--proto_path={}".format(x))

    for proto_file in proto_files:
        command = (
            [
                "grpc_tools.protoc",
                "--proto_path={}".format(inclusion_root),
                "--proto_path={}".format(well_known_protos_include),
            ]
            + includes
            + [
                "--python_out={}".format(inclusion_root),
                "--grpc_python_out={}".format(inclusion_root),
            ]
            + [proto_file]
        )
        if protoc.main(command) != 0:
            raise Exception("error: {} failed".format(command))


class BuildProtosCommand(Command):
    user_options = []  # type: ignore

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        proto_files_root = Path("protos")
        build_package_protos(proto_files_root)

        for proto_def in proto_files_root.rglob("*.proto"):
            proto_def_new = Path("py", *proto_def.parts[1:])
            shutil.copy(proto_def, proto_def_new)

        for proto_file in proto_files_root.rglob("*_pb2*.py"):
            proto_file_new = Path("py", *proto_file.parts[1:])
            if not proto_file_new.exists():
                proto_file.rename(proto_file_new)
            if proto_file.exists():
                proto_file.unlink()
        for proto_file in proto_files_root.rglob("*_pb2*.pyi"):
            proto_file_new = Path("py", *proto_file.parts[1:])
            if not proto_file_new.exists():
                proto_file.rename(proto_file_new)
            if proto_file.exists():
                proto_file.unlink()


class CleanFilesCommand(Command):
    user_options = []  # type: ignore

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        proto_files_root = Path("py/farm_ng")
        for proto_def in proto_files_root.rglob("*.proto"):
            assert proto_def.unlink() is None
        for proto_file in proto_files_root.rglob("*_pb2*.py"):
            assert proto_file.unlink() is None
        for proto_file in proto_files_root.rglob("*_pb2*.pyi"):
            assert proto_file.unlink() is None


class BuildProtosInstall(install):
    def run(self):
        # 1. Build the protobufs
        BuildProtosCommand.run(self)
        # 2. Run the installation
        install.run(self)
        # 3. Clean the generated protobufs
        CleanFilesCommand.run(self)


class BuildProtosDevelop(develop):
    def run(self):
        # 1. Build the protobufs
        BuildProtosCommand.run(self)
        # 2. Run the installation
        develop.run(self)


class BuildProtosEggInfo(egg_info):
    def run(self):
        # 1. Build the protobufs
        BuildProtosCommand.run(self)
        # 2. Run the installation
        egg_info.run(self)


def makeCmdClass():
    return {
        "build_package_protos": BuildProtosCommand,
        "install": BuildProtosInstall,
        "develop": BuildProtosDevelop,
        "egg_info": BuildProtosEggInfo,
        "clean": CleanFilesCommand,
    }
