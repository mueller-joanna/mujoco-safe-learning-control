#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

build_dir=./debug
if [ -d "$build_dir" ]; then
    rm -r ${build_dir}/
fi

mkdir $build_dir
cd $build_dir 
cmake -DCMAKE_BUILD_TYPE=Debug .. 
make
