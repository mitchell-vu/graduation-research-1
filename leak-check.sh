#!/bin/bash
clear
printf '\e[3J'
if [ -d "build" ]; then
    rm -rf build
fi
target=$1
cmake -S . -B build
cmake --build build --config Debug --target $target
if [ ! -d "out" ]; then
  mkdir out
fi
leaks -atExit -- ./build/$target