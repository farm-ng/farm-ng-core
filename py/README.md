# farm-ng-core [python]

## Install

Install `pip3` & `virtualenv`:

```bash
sudo apt-get install python3-pip
sudo pip3 install virtualenv
```

Clone the project:

```bash
git clone https://github.com/farm-ng/farm-ng-core.git
```

Move to the python code directory:

```bash
cd farm-ng-core/py
```

Start a virtual environment:

```bash
# assuming you're already in farm-ng-core/py directory
python3 -m venv venv
source venv/bin/activate
```

Create and install the `farm_ng.core` package:

```bash
# install to system
pip3 install .

# or for development mode
pip3 install -e .[dev]
```

Verify that you have installed the package

```bash
python3 -c "import farm_ng; print(farm_ng.__version__)"
```
