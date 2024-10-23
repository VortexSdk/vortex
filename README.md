# Vortex

The **Vortex Project** is a high-performance, low-level C++ framework that focuses on memory management, system calls, and efficient I/O operations using `io_uring`. It also includes several test files to demonstrate basic system functionalities such as memory allocation, string handling, and system map interactions.

This project is built with modern C++ standards (C++23) and leverages Clang-19 compiler features. It contains a variety of tests and system wrappers designed for optimal performance and usage in freestanding environments, i.e., without relying on standard libraries.

## Features

- **System Call Wrappers**: Low-level syscall abstractions for Linux, covering common operations such as file handling, memory management, and I/O operations using `io_uring`.
- **Memory Management**: Custom memory allocators and page management.
- **I/O Operations**: Efficient I/O using `io_uring` with detailed error handling.

## Motivation

This project is built with performance as a top priority, running without dependencies on libc or libc++. For enhanced efficiency, it avoids both C++'s RAII and exceptions, using a custom error-handling and allocator system instead. Pinned heap memory ensures memory safety, while ubsan support helps detect bugs at runtime. Additionally, the project leverages SIMD to accelerate data processing.

## Prerequisites

To build this project, you need:

- **Clang 19** or newer
- **GCC 14** or newer
- C++23 support
- A Linux system with `io_uring` support (6.0 or newer)

## Test run

Run the tests by executing:
```bash
./build.sh test
```

Or run them with optimizations enabled:
```bash
./build.sh test release
```

If you want to compiler with GCC, just pass the `COMPILER` environment variable when executing `build.sh`:
```bash
COMPILER="gcc" ./build.sh test
```

## Tidy

Lint the project by executing:
```bash
./build.sh lint
```

Format the project by executing:
```bash
./build.sh fmt
```
