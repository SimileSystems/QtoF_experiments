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

# Clone OF as submodule
if [ ! -d ${bd}/of ] ; then 
    cd ${bd}
    git submodule add -b qt-integration git@github.com:SimileSystems/openFrameworks.git of
fi

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

# Stopping here for now as the QtOf repository has been setup.
exit

# Create unique name for this build type.
cd ${d}
bd="${d}/${build_dir}.${cmake_build_type}"

if [ ! -d ${bd} ] ; then 
    mkdir ${bd}
fi

# Compile libuv; used for an example that was requested by James George
if [ ! -d ${d}/libuv.build ] ; then

    if [ ! -d ${ed}/extern/lib ] ; then
        mkdir ${ed}/extern/lib
    fi

    mkdir ${d}/libuv.build
    cd ${d}/libuv.build

    git clone --depth 1 https://github.com/libuv/libuv .

    if [ ! -d build/gyp ] ; then
        git clone --depth 1 https://chromium.googlesource.com/external/gyp.git build/gyp
    fi

    if [ "${os}" = "mac" ] ; then
        ./gyp_uv.py -f xcode -D prefix=${ed}/extern
        xcodebuild -ARCHS="x86_64" \
                   -project uv.xcodeproj \
                   -configuration Release \
                   -target All

        if [ ! -f ${ed}/extern/lib/clang/libuv.a ] ; then
            cp ${d}/libuv.build/build/Release/libuv.a ${ed}/extern/lib/clang
        fi
    fi
    
    if [ ! -f ${ed}/extern/include/uv.h ] ; then 
        cp ${d}/libuv.build/include/*.h ${ed}/extern/include/
    fi
fi

# Compile the library.
cd ${bd}
cmake -DCMAKE_INSTALL_PREFIX=${id} \
      -DCMAKE_BUILD_TYPE=${cmake_build_type} \
      -DQT_PATH=${QT_PATH} \
      -G "${cmake_generator}" \
      ..

if [ $? -ne 0 ] ; then
    exit
fi

cmake --build . --config ${cmake_build_type} --target install

if [ $? -ne 0 ] ; then
    exit
fi
set -x
cd ${id}/bin
if [ "${os}" == "mac" ] ; then
    cd ./test_qt.app/Contents/MacOs
    ./test_qt
    #open ./test_qt.app
else
    ${debugger} ./test_qt${debug_flag}
fi


