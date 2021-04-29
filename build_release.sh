#!/bin/sh
# This script performs a clean release build on Linux

set -e

rm -rf build
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_MAIN=ON ..
make
cd ..
