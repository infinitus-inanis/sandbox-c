#!/bin/bash
set -e
pushd $(dirname $0) &> /dev/null

CMAKE_FLAGS=(
  "-DCMAKE_BUILD_TYPE=Debug"
  "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"
  "-DASAN_STATIC=1"
)

function __build() {
  [ -d build ] || {
    mkdir build
  }
  pushd build &> /dev/null
    cmake .. ${CMAKE_FLAGS[@]}
    make -j$(nproc --all)
  popd &> /dev/null
}

function __run() {
  [ -d build ] && {
    ./build/algo "$@"
  }
}

function __clean() {
  [ -d build ] && {
    rm -r build
  }
}

function __debug() {
  [ -d build ] && {
    gdb build/algo
  }
}

case $1 in
  build) { __build; };;
  run)   { __run "${@:2}"; };;
  clean) { __clean; };;
  debug) { __debug "${@:2}"; };;
  all|*)   { 
    echo "== build =="
    __build;
    echo
    echo "== start =="
    __run "${@:2}"; 
    echo
  };;
esac

popd &> /dev/null