 #!/bin/bash

set -x # echo on
set -e # exit on error

rm -rf projects
mkdir projects
cd projects
git clone https://github.com/jothepro/doxygen-awesome-css.git
cd ..

doxygen doxyfile
