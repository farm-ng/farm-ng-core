from setuptools import setup

# hack for bootstrapping and include farm_ng.core.package from it's setup.py
import pathlib
from pathlib import Path

from setuptools import Command
from setuptools.command.develop import develop
from setuptools.command.egg_info import egg_info
from setuptools.command.install import install

farm_ng_core_path = pathlib.Path(__file__).parent.resolve() / "py"
import sys

sys.path.insert(0, str(farm_ng_core_path))


class BuildProtosCommand(Command):
    user_options = []  # type: ignore

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        from farm_ng.core.package import build_package_protos

        build_package_protos(proto_root=Path("protos"), package_root=Path("py"))


class CleanFilesCommand(Command):
    user_options = []  # type: ignore

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        from farm_ng.core.package import clean_protos

        clean_protos(package_root=Path("py"))


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


setup(
    cmdclass={
        "build_package_protos": BuildProtosCommand,
        "install": BuildProtosInstall,
        "develop": BuildProtosDevelop,
        "egg_info": BuildProtosEggInfo,
        "clean": CleanFilesCommand,
    }
)
