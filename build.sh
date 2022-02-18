#!/bin/bash

build_directory=build
mkdir -p ${build_directory}
cd ${build_directory}
cmake ..

if [ $# -ne 1 ]; then
  make
else
  make $1
fi
