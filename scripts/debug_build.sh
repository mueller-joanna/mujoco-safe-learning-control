#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

build_dir=./debug
if [ -d "$build_dir" ]; then
    rm -rf ${build_dir}/
fi

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B $build_dir
cmake --build $build_dir
