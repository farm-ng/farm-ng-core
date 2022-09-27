# Python farm-ng-core

## Install

```bash
pip install farm_ng
```

## Development

Create first a virtual environent

```bash
python3 -m venv venv
source venv/bin/activate
```

Compile protos and install the package in editable mode

```bash
pip install -e .[dev]
```

## Test

```bash
pytest -v tests/
```
