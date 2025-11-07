#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

build_dir=./build
if [ -d "$build_dir" ]; then
    read -p "Remove previous build files? [y/n] " -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        rm -rf ${build_dir}/*
    fi
fi

torch_path=$(pwd)/vendors/libtorch/
# From https://lindevs.com/clean-build-directory-using-cmake
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release \
-DCMAKE_PREFIX_PATH="${torch_path}" --fresh -S . -B ${build_dir}
cmake --build build 
