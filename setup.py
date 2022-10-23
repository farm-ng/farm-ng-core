from setuptools import setup

# hack for bootstrapping and include farm_ng.core.package from it's setup.py
import pathlib

farm_ng_core_path = pathlib.Path(__file__).parent.resolve() / "py"
import sys

sys.path.insert(0, str(farm_ng_core_path))

# normal farm_ng packages will just do the following, assuming they depend on farm_ng.core
from farm_ng.core.package import makeCmdClass

setup(cmdclass=makeCmdClass())
