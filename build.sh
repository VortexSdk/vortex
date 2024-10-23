#!/bin/bash

set -euo pipefail  # Enables strict error handling: stop on error, undefined vars, and fail on pipe error.

# Can either use clang or gcc.
readonly BUILD_DIR="build"
readonly PROJECT_NAME="vortex"
readonly COMPILER="${COMPILER:-clang}" 
readonly INCLUDES=("-I." "-Ibuild/uapi")
readonly OUTPUT="$BUILD_DIR/$PROJECT_NAME"
readonly TEST_FILES=$(find tests/ -type f -name "*.cpp")
readonly ALL_FILES=$(find . -type f -name "*.*pp" | grep -v "build")
BUILD_FLAGS=("-ffreestanding" "-fno-builtin" "-nostdlib" "-nostdlib++" "-nostdinc" "-nostdinc++" "-std=c++23" "-Werror" "-fno-exceptions" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic" "-Wno-c++20-extensions" "-Wno-padded" "-Wno-reserved-macro-identifier" "-Wno-unsafe-buffer-usage" "-Wno-unused-function" "-Wno-gnu-anonymous-struct" "-Wno-nested-anon-types" "-Wno-vla-cxx-extension" "-Wno-zero-length-array" "-Wno-flexible-array-extensions" "-Wno-unused-template" "-Wno-zero-as-null-pointer-constant" "-fno-signed-char" "-fno-use-cxa-atexit" "-fdata-sections" "-ffunction-sections" "-Wl,--gc-sections")

readonly GCC_EXTRA_BUILD_FLAGS=("-Wall" "-Wextra" "-nostartfiles" "-fno-stack-protector")
readonly CLANG_EXTRA_BUILD_FLAGS=("-Weverything" "-nostdlibinc" "-fno-knr-functions")
readonly GCC_SANITIZER_FLAG=("-fsanitize=undefined,float-divide-by-zero,signed-integer-overflow")
readonly CLANG_SANITIZER_FLAG=("-fsanitize=undefined,nullability,float-divide-by-zero,unsigned-integer-overflow,implicit-conversion,local-bounds")

if [ $COMPILER == "clang" ];then
    BUILD_FLAGS+=("${CLANG_EXTRA_BUILD_FLAGS[@]}")
else
    if [ $COMPILER == "gcc" ]; then
        BUILD_FLAGS+=("${GCC_EXTRA_BUILD_FLAGS[@]}")
    else
        echo "Invalid compiler `$COMPILER`!"
        exit 1
    fi
fi

# Utility function to print and execute commands.
exec_n_print() {
    echo "$*"
    "$@"
}

# Detect if the argument is an optimization level.
is_optimization_level() {
    case "$1" in
        "debug" | "release" | "small")
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Build and run tests.
build_and_run_tests() {
    local optimization_level=""
    local strip_symbols=""
    local test_file=""
    local test_files=()

    # Extract arguments.
    while [[ $# -gt 0 ]]; do
        case "$1" in
            "debug" | "release" | "small")
                optimization_level="$1"
                shift
                ;;
            "strip")
                strip_symbols="strip"
                shift
                ;;
            *)
                test_file="tests/$1.cpp"
                if [[ -f "$test_file" ]]; then
                    test_files+=("$test_file")
                else
                    echo "Test file '$test_file' not found!" >&2
                    exit 1
                fi
                shift
                ;;
        esac
    done

    # Validate and process optimization level.
    local build_flags=("${BUILD_FLAGS[@]}")
    case "$optimization_level" in
        "release")
            build_flags+=("-O3" "-flto" "-fno-rtti" "-static")
            ;;
        "small")
            build_flags+=("-Oz" "-flto" "-fno-rtti" "-static")
            ;;
        "")
            build_flags+=("-O0" "-g" "-lubsan" "-lc" "-lgcc_s" "-lstdc++") # Default to debug

            if [ $COMPILER == "clang" ];then
                build_flags+=("${CLANG_SANITIZER_FLAG[@]}")
            else
                if [ $COMPILER == "gcc" ]; then
                    build_flags+=("${GCC_SANITIZER_FLAG[@]}")
                fi
            fi
            ;;
        *)
            echo "Invalid optimization level!" >&2
            exit 1
            ;;
    esac

    mkdir -p "$BUILD_DIR"

    if [[ ${#test_files[@]} -gt 0 ]]; then
        # Compile and run the specified tests, creating unique output files for each.
        for file in "${test_files[@]}"; do
            local test_output="$BUILD_DIR/$(basename "$file" .cpp)_test"
            exec_n_print "$COMPILER" "$file" -o "$test_output" "${INCLUDES[@]}" "${build_flags[@]}"
            "$test_output"
        done
    else
        # Compile and run all tests, creating unique output files for each.
        for file in $TEST_FILES; do
            local test_output="$BUILD_DIR/$(basename "$file" .cpp)_test"
            exec_n_print "$COMPILER" "$file" -o "$test_output" "${INCLUDES[@]}" "${build_flags[@]}"
            exec_n_print "$test_output"
        done
    fi

    if [[ "$strip_symbols" == "strip" ]]; then
        for file in "${test_files[@]}"; do
            local test_output="$BUILD_DIR/$(basename "$file" .cpp)_test"
            exec_n_print strip -s "$test_output"
        done
    fi
}

delete_array_element() {
    local word=$1      # the element to search for & delete
    local aryref="$2[@]" # a necessary step since '${!$2[@]}' is a syntax error
    local arycopy=("${!aryref}") # create a copy of the input array
    local status=1

    for (( i = ${#arycopy[@]} - 1; i >= 0; i-- )); do # iterate over indices backwards
        elmt=${arycopy[$i]}
        [[ $elmt == $word ]] && unset "$2[$i]" && status=0 # unset matching elmts in orig. ary
    done

    return $status # return 0 if something was deleted; 1 if not
}

# Build a compile_commands.json for tools like clangd.
build_command_db() {
    local build_flags=("${BUILD_FLAGS[@]}" "-O0")

    delete_array_element "-Wl,--gc-sections" build_flags

    mkdir -p "$BUILD_DIR/commands_db" "$BUILD_DIR/tmp"

    find tests/ -type f -name "*.cpp" | while read -r filename; do
        local base_name
        base_name=$(basename "$filename")
        exec_n_print "$COMPILER" -MJ "$BUILD_DIR/commands_db/$base_name.o.json" -o "$BUILD_DIR/tmp/$base_name.pch" "$filename" "${build_flags[@]}" "${INCLUDES[@]}"
    done

    # Create the final compile_commands.json.
    sed -e '1s/^/[\'$'\n''/' -e '$s/,$/\'$'\n'']/' "$BUILD_DIR/commands_db/"*.o.json > compile_commands.json
}

sysdef_gen() {
    exec_n_print "$COMPILER" -lstdc++ -std=c++23 vortex/linux/syscall/wrapperGenerator.cpp -o build/wrapperGenerator
    ./build/wrapperGenerator
}

# Clean the build directory.
clean() {
    exec_n_print rm -rf "$BUILD_DIR"
}

# Main command dispatcher.
main() {
    case "${1:-}" in
        "test")
            shift
            build_and_run_tests "$@"
            ;;
        "clean")
            clean
            ;;
        "command-db-gen")
            build_command_db
            ;;
        "sysdef-gen")
            sysdef_gen
            ;;
        "uapi")
            exec vortex/linux/patch.sh
            ;;
        "lint")
            exec_n_print clang-tidy --use-color $TEST_FILES
            ;;
        "fmt")
            exec_n_print clang-format -i $ALL_FILES
            ;;
        "fmt-check")
            exec_n_print clang-format --dry-run --Werror $ALL_FILES
            ;;
        *)
            echo "Usage: $0 {test|clean|command-db-gen|sysdef-gen|uapi} [options]" >&2
            ;;
    esac
}

main "$@"

