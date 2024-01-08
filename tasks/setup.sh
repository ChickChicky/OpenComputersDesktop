#!/usr/bin/env bash

PLAT=${1-$OSTYPE}

( # Lua installation

    if [[ "$PLAT" == "msys" ]]; then
        LUA_PLATFORM="mingw"
    elif [[ "$PLAT" == "linux-gnu" ]]; then
        LUA_PLATFORM="linux"
    else
        LUA_PLATFORM=$PLAT
    fi

    LUA_VER="5.4.6"
    LUA_SOURCE=lua-$LUA_VER
    LUA_SOURCE_COMPRESSED=$LUA_SOURCE.tar.gz

    wget https://www.lua.org/ftp/$LUA_SOURCE_COMPRESSED
    tar zxf $LUA_SOURCE_COMPRESSED

    (
        cd $LUA_SOURCE
        make $LUA_PLATFORM test
    )

)

( # Raylib installation

    git clone https://github.com/raysan5/raylib.git

    (
        cd ./raylib/src
        make PLATFORM=PLATFORM_DESKTOP
    )

)