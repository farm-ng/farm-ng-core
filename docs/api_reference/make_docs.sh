 #!/bin/bash

set -x # echo on
set -e # exit on error

rm -rf projects
rm -rf html
mkdir projects
cd projects
git clone https://github.com/jothepro/doxygen-awesome-css.git
cd ..

doxygen doxyfile
