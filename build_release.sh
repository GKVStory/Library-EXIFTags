#!/bin/sh
# This script performs a clean release build on Linux

set -e

if [ -n "$JENKINS_HOME" ]; then
    # This is required to avoid some subtle issue between Jenkins-as-SYSTEM using the Conan
    # setup as installed by the tgrobotics user. Probably a permissions thing.
    # Also Conan suggests that its cache is not suitable for concurrent access like a CI server
    # produces, although that may be out of date. Haven't confirmed.
    export CONAN_USER_HOME="$WORKSPACE"
fi

rm -rf build
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_MAIN=ON ..
make
cd ..
