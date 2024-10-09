#!/usr/bin/bash


# Build function
function build() {
    echo "Building chess exec"
    @echo off
    mkdir -p build
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build

    # Makes it easy to run from command line
    cp build/chess .
}

# Debug function
function debug() {
    echo "Building debug chess exec"
    @echo off
    mkdir -p build
    cmake -S . -B debug -DCMAKE_BUILD_TYPE=Debug
    cmake --build debug

    # Makes it easy to run from command line
    cp debug/chess .
}

# check if there are no args
if [ "$#" -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi

# Loop over arguments
# If 'build', run build func, if 'debug', run debug func

for arg in "$@"
do
    if [ "$arg" == "build" ]; then
        build
    elif [ "$arg" == "debug" ]; then
        debug
    fi
done