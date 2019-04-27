#!/usr/bin/env bash

set -e

# make sure that we are under project folder
mkdir -p build
pushd build

cmake ..
make -j `grep -c ^processor /proc/cpuinfo`

popd

unamestr="$(uname)"
if [[ "$unamestr" == "Linux" ]]; then
    LIBRARY_NAME_SUFFIX=so
elif [[ "$unamestr" == "Darwin" ]]; then
    LIBRARY_NAME_SUFFIX=dylib
else
    LIBRARY_NAME_SUFFIX=dll
fi

# install to build/lib
mkdir -p build/lib/lib64
cp -r include/ build/lib/include/
mv build/libdsa.$LIBRARY_NAME_SUFFIX build/lib/lib64/libdsa.$LIBRARY_NAME_SUFFIX
