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
    fi
done

# Download our OF version
if [ ! -d ${of} ] ; then
    mkdir ${of}
    cd ${of}
    git clone --depth 1 git@github.com:SimileSystems/openFrameworks.git -b qt-integration .
    git submodule init
    git submodule update
fi

if [ ! -d ${of}/libs/boost ] ; then
    if [ "${os}" == "mac" ] ; then
        cd ${of}/scripts/osx
        ./download_libs.sh
    elif [ "${os}" == "win" ] ; then
        cd ${of}/scripts/vs/
        ./download_libs.sh
    fi
fi

# Compile libuv; used for an example that was requested by James George
if [ ! -d ${d}/libuv.build ] ; then

    if [ ! -d ${ed}/extern/lib ] ; then
        mkdir ${ed}/extern/lib
    fi

    if [ ! -d ${d}/libuv.build ] ; then
        mkdir ${d}/libuv.build
    fi
    
    cd ${d}/libuv.build
    git clone https://github.com/libuv/libuv .
    
    if [ ! -d build/gyp ] ; then
        git clone https://chromium.googlesource.com/external/gyp.git build/gyp
    fi

    if [ "${os}" == "mac" ] ; then
       # sh ./autogen.sh
        ./gyp_uv.py -f xcode -D prefix=${ed}/extern
        xcodebuild -ARCHS="x86_64" \
                   -project uv.xcodeproj \
                   -configuration Release \
                   -target All

        if [ ! -f ${ed}/extern/lib/libuv.a ] ; then
            cp ${d}/libuv.build/build/Release/libuv.a ${ed}/extern/lib
        fi
    elif [ "${os}" == "win" ] ; then
        if [ ! -f ${d}/libuv.build/Release/libuv.lib ] ; then
            echo ""
            echo "------------------------------------------------------"
            echo ""
            echo "1. Open the Developer Command Prompt for VS2015"
            echo "2. cd ${d}/libuv.build "
            echo "3. set PYTHON=c:\Python2.7\python.exe"
            echo "4. vcbuild.bat"
            echo "5. msbuild uv.sln /t:libuv"
            echo ""
            echo "------------------------------------------------------"
            echo ""
            exit
        fi
    fi
    
    if [ ! -f ${ed}/extern/include/uv.h ] ; then 
        cp ${d}/libuv.build/include/*.h ${ed}/extern/include/
    fi
fi

# Create unique name for this build type.
bd="${d}/${build_dir}.${cmake_build_type}"

if [ ! -d ${bd} ] ; then 
    mkdir ${bd}
fi

# Compile the library.
cd ${bd}
cmake -DCMAKE_INSTALL_PREFIX=${id} \
      -DCMAKE_BUILD_TYPE=${cmake_build_type} \
      -DOF_DIR=${of}/ \
      -G "${cmake_generator}" \
      ..

if [ $? -ne 0 ] ; then
    exit
fi

cmake --build . --config ${cmake_build_type} --target install

if [ $? -ne 0 ] ; then
    exit
fi

cd ${id}/bin
${debugger} ./test_qt${debug_flag}
