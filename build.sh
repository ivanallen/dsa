#!/usr/bin/env bash

set -e

# get system type
unamestr="$(uname)"
if [[ "$unamestr" == "Linux" ]]; then
    LIBRARY_NAME_SUFFIX=so
elif [[ "$unamestr" == "Darwin" ]]; then
    LIBRARY_NAME_SUFFIX=dylib
else
    LIBRARY_NAME_SUFFIX=dll
fi

# make sure that we are under project folder
mkdir -p build
pushd build

cmake .. "-DDSA_BUILD_TESTS=true"

if [[ "$unamestr" == "Darwin" ]]; then
    make -j `sysctl -n machdep.cpu.thread_count`
else
    make -j `grep -c ^processor /proc/cpuinfo`
fi

popd
