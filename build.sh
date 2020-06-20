#!/bin/bash
PROJECT_DIR=$PWD
cd ..
mkdir -p build
cd build
cmake "$PROJECT_DIR" -G  "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug 
