#!/bin/bash

# This script should be called from the root directory like `./vortex/linux/patch.sh`.

VERSION="6.10"
CLANG_FORMAT=".clang-format"

mkdir -p build/uapi

if [ ! -f "build/uapi/v$VERSION.tar.gz" ]; then
    wget "https://github.com/torvalds/linux/archive/refs/tags/v$VERSION.tar.gz" -P build/uapi
fi

if [ ! -d "build/uapi/linux-$VERSION" ]; then
    tar -xvf "build/uapi/v$VERSION.tar.gz" -C build/uapi/
fi

mkdir -p "build/uapi/asm"
mkdir -p "build/uapi/linux"
mkdir -p "build/uapi/asm-generic"
make -C "build/uapi/linux-$VERSION" headers_install HDR_ARCH_LIST=x86_64,arm64 INSTALL_HDR_PATH=".."

cat "vortex/linux/headers.txt" | while read header; do
    if [[ ${header:0:1} == '#' || ${header:0:1} == ' ' ]]; then
        continue
    fi

    if [ -f "build/uapi/include/asm/$header" ]; then
        cp "build/uapi/include/asm/$header" "build/uapi/asm/$header"
        clang-format --style=file:"$CLANG_FORMAT" -i "build/uapi/asm/$header"
    fi

    if [ -f "build/uapi/include/asm-generic/$header" ]; then
        cp "build/uapi/include/asm-generic/$header" "build/uapi/asm-generic/$header"
        clang-format --style=file:"$CLANG_FORMAT" -i "build/uapi/asm-generic/$header"
    fi

    if [ -f "build/uapi/include/linux/$header" ]; then
        cp "build/uapi/include/linux/$header" "build/uapi/linux/$header"
        clang-format --style=file:"$CLANG_FORMAT" -i "build/uapi/linux/$header"
    fi
done

# rm -Rf "include"
