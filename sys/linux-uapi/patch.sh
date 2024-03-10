#!/bin/bash

VERSION=$(<version.txt)

if [ ! -f "v$VERSION.tar.gz" ]; then
    wget "https://github.com/torvalds/linux/archive/refs/tags/v$VERSION.tar.gz"
fi

if [ ! -d "linux-$VERSION" ]; then
    tar -xvf "v$VERSION.tar.gz"
fi

mkdir -p "asm"
mkdir -p "linux"
mkdir -p "asm-generic"
make -C "linux-$VERSION" headers_install HDR_ARCH_LIST=x86_64,arm64 INSTALL_HDR_PATH=".."

cat "headers.txt" | while read header; do
    if [[ ${header:0:1} == '#' || ${header:0:1} == ' ' ]]; then
        continue
    fi

    if [ -f "include/asm/$header" ]; then
        cp "include/asm/$header" "asm/$header"
        clang-format --style=file:../../.clang-format -i "asm/$header"
    fi

    if [ -f "include/asm-generic/$header" ]; then
        cp "include/asm-generic/$header" "asm-generic/$header"
        clang-format --style=file:../../.clang-format -i "asm-generic/$header"
    fi

    if [ -f "include/linux/$header" ]; then
        cp "include/linux/$header" "linux/$header"
        clang-format --style=file:../../.clang-format -i "linux/$header"
    fi
done

rm -Rf "include"
