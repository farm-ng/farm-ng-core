import os
import sys
from pathlib import Path
import stat


def build_package_protos(proto_root, package_root, proto_includes=[]):
    """
    includes: additional include paths
    """
    from grpc_tools import protoc
    import pkg_resources

    proto_files = []
    inclusion_root = os.path.abspath(proto_root)
    package_root = os.path.abspath(package_root)

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
                "--python_out={}".format(package_root),
                "--grpc_python_out={}".format(package_root),
            ]
            + [proto_file]
        )
        if protoc.main(command) != 0:
            raise Exception("error: {} failed".format(command))

        proto_file_new = package_root / Path(proto_file).relative_to(inclusion_root)
        Path(proto_file_new).parent.mkdir(parents=True, exist_ok=True)
        with open(
            proto_file_new,
            "wt",
        ) as proto_out:
            proto_out.write("// This file generated for packaging, do not edit.\n")
            proto_out.write(f"// To edit, see original source:\n// {proto_file}.\n")
            proto_out.write(f"//\n")
            with open(proto_file, "rt") as proto_in:
                proto_out.write(proto_in.read())


def clean_protos(package_root):
    for proto_def in package_root.rglob("*.proto"):
        assert proto_def.unlink() is None
    for proto_file in package_root.rglob("*_pb2*.py"):
        assert proto_file.unlink() is None
    for proto_file in package_root.rglob("*_pb2*.pyi"):
        assert proto_file.unlink() is None
