#!/bin/bash

set -euo pipefail  # Enables strict error handling: stop on error, undefined vars, and fail on pipe error.

readonly PROJECT_NAME="vortex"
readonly BUILD_DIR="build"
readonly OUTPUT="$BUILD_DIR/$PROJECT_NAME"
readonly INCLUDES=("-I." "-Ibuild/uapi")
readonly BUILD_FLAGS=("-ffreestanding" "-fno-builtin" "-nostdlib" "-nostdlib++" "-Wno-missing-noreturn" "-std=c++23" "-Weverything" "-Werror" "-fno-exceptions" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic" "-Wno-missing-prototypes" "-Wno-reserved-identifier" "-Wno-unsafe-buffer-usage" "-Wno-unused-function" "-fno-signed-char" "-fno-use-cxa-atexit" "-fno-rtti" "-fno-knr-functions" "-nostdinc" "-nostdinc++" "-nostdlibinc")

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
        "debug")
            build_flags+=("-O0" "-g")
            ;;
        "release")
            build_flags+=("-Ofast" "-flto")
            ;;
        "small")
            build_flags+=("-Os" "-flto")
            ;;
        "")
            build_flags+=("-O0")  # Default to debug if no optimization level is specified.
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
            exec_n_print clang++ "$file" -o "$test_output" "${build_flags[@]}" "${INCLUDES[@]}"
            "$test_output"
        done
    else
        # Find all test files and run them.
        local test_files_found
        test_files_found=$(find tests/ -type f -name "*.cpp")

        if [[ -z "$test_files_found" ]]; then
            echo "No test files found in 'tests/' directory!" >&2
            exit 1
        fi

        # Compile and run all tests, creating unique output files for each.
        for file in $test_files_found; do
            local test_output="$BUILD_DIR/$(basename "$file" .cpp)_test"
            exec_n_print clang++ "$file" -o "$test_output" "${build_flags[@]}" "${INCLUDES[@]}"
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


# Build a compile_commands.json for tools like clangd.
build_command_db() {
    local build_flags=("${BUILD_FLAGS[@]}" "-O0")

    mkdir -p "$BUILD_DIR/commands_db" "$BUILD_DIR/tmp"

    find tests/ -type f -name "*.cpp" | while read -r filename; do
        local base_name
        base_name=$(basename "$filename")
        exec_n_print clang++ -MJ "$BUILD_DIR/commands_db/$base_name.o.json" -o "$BUILD_DIR/tmp/$base_name.pch" "$filename" "${build_flags[@]}" "${INCLUDES[@]}"
    done

    # Create the final compile_commands.json.
    sed -e '1s/^/[\'$'\n''/' -e '$s/,$/\'$'\n'']/' "$BUILD_DIR/commands_db/"*.o.json > compile_commands.json
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
        "command_db")
            build_command_db
            ;;
        "uapi")
            exec vortex/linux/patch.sh
            ;;
        "lint")
            clang-tidy **/*.hpp **/*.cpp
            ;;
        "fmt")
            clang-format -i **/*.hpp **/*.cpp
            ;;
        "fmt-check")
            clang-format --dry-run --Werror **/*.hpp **/*.cpp
            ;;
        *)
            echo "Usage: $0 {test|clean|command_db|uapi} [options]" >&2
            ;;
    esac
}

main "$@"

