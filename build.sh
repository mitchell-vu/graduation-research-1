#!/bin/bash
clear
printf '\e[3J'
rm -rf build
target=$1
cmake -S . -B build
cmake --build build --config Debug --target $target
./build/$target