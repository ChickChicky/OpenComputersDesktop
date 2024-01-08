#!/usr/bin/env bash

SOURCE="./src"
OUTPUT="ocd"

WARNINGS="-Wall -Wextra"
LIBS="-llua -lraylib -lm -ldl -lpthread"

LUA="./lua-5.4.6"

gcc $SOURCE/*.c $SOURCE/**/*.c $WARNINGS $LIBS $(find ./src/ -type d -printf "-I %p ") -L $LUA/src -I $LUA/src -L ./raylib/src/ -I ./raylib/src/ -o $OUTPUT