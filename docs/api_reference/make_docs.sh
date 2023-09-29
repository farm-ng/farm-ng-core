 #!/bin/bash

set -x # echo on
set -e # exit on error

rm -rf projects
rm -rf html
mkdir projects
cd projects
git clone https://github.com/jothepro/doxygen-awesome-css.git
cd ..


# Fail if undocumented (cpp, farm-ng-core)
doxygen doxyfile_cpp_farm_ng_warn_as_error

# Generate doxygen docs
doxygen doxyfile
