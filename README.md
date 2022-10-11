# farm-ng-core

## Build on MacOS

Make sure ports is installed: https://www.macports.org/install.php

Install dependencies, once:

```
sudo port install cmake
sudo PATH=$PATH port install protobuf3-cpp
sudo port install ccache
sudo port install pre-commit
sudo port select --set virtualenv virtualenv310
```

From within the repo:

```
mkdir build
cd build
cmake ../super_project
make -j
```

Run pre-commit before commiting changes

```
pre-commit run -a
```
