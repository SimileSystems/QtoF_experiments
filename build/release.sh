#!/bin/sh
d=${PWD}
bd=${d}/../
id=${bd}/install
ed=${d}/../
rd=${d}/../reference/
of=${d}/../of
d=${PWD}
is_debug="n"
build_dir="build_unix"
cmake_build_type="Release"
cmake_config="Release"
debug_flag=""
debugger=""
cmake_generator=""


# Detect OS.
if [ "$(uname)" == "Darwin" ]; then
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Unix Makefiles"
    fi
    os="mac"
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Unix Makefiles"
    fi
    os="linux"
else
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Visual Studio 14 2015 Win64"
        build_dir="build_vs2015"
    fi
    os="win"
fi

# Detect Command Line Options
for var in "$@"
do
    if [ "${var}" = "debug" ] ; then
        is_debug="y"
        cmake_build_type="Debug"
        cmake_config="Debug"
        debug_flag="_debug"
        debugger="lldb"
    elif [ "${var}" = "xcode" ] ; then
        build_dir="build_xcode"
        cmake_generator="Xcode"
        build_dir="build_xcode"
    elif [ "${var}" = "vs2013" ] ; then
        build_dir="build_vs2013"
        compiler="vs2013"
        cmake_generator="Visual Studio 12 2013 Win64"
    fi
done

if [ -d ${bd}/of ] ; then
    git submodule init
    git submodule update
fi

if [ ! -d ${of}/libs/boost ] ; then
    if [ "${os}" == "mac" ] ; then
        cd ${of}/scripts/osx
        ./download_libs.sh
    elif [ "${os}" == "win" ] ; then
        cd ${of}/scripts/vs
        /c/Windows/System32/WindowsPowerShell/v1.0/powershell -executionPolicy bypass -file "${of}/scripts/vs/download_libs.ps1"
    fi
fi

