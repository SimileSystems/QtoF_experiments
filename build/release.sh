#!/bin/sh
d=${PWD}
bd=${d}/../
of=${d}/../of

# Detect OS.
if [ "$(uname)" == "Darwin" ]; then
    os="mac"
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
    os="linux"
else
    os="win"
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

