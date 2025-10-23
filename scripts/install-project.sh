#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

build_dir=./build
if [ -d "$build_dir" ]; then
    read -p "Remove previous build files? [y/n] " -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        rm -r ${build_dir}/*
    fi
fi


# From https://lindevs.com/clean-build-directory-using-cmake
cmake -S . -B build
cmake --build build