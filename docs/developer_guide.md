# Developer Guide

PlayArchive is maintained and developed by **Limitless Protocols**.

## Prerequisites

To build PlayArchive, you need:
- **C++ Compiler**: Support for C++17 (MSVC 2019+, GCC 9+, Clang 10+).
- **CMake**: Version 3.16 or higher.
- **Qt 6**: Specifically the Core, Gui, Qml, Quick, and Concurrent modules. (Must be installed on the system).
- **LibArchive**: (Must be installed on the system).

### Automated Dependencies
The following libraries will be automatically downloaded and built by CMake using `FetchContent` if they are not found on your system:
- `Zstandard (Zstd)`
- `xxHash`
- `GoogleTest` (for testing)

## Build System

We use CMake as our build system. The project is organized into subdirectories for `core`, `gui`, and `cli`.

### Building

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Adding a New Format

To support a new archive format:
1.  Create a new class in `src/core` that inherits from `IArchiveHandler`.
2.  Implement `create`, `extract`, and `list` methods.
3.  Register your handler in `src/gui/main.cpp` and `src/cli/main.cpp` using `FormatManager::instance().registerHandler("ext", your_handler)`.

## Running Tests

We use GoogleTest for unit testing the core engine.

```bash
cd build
ctest # or run the playarchive_tests executable
```

## UI Customization

The GUI is written in QML located in `src/gui/Main.qml`. We use a custom dark theme. To modify the look and feel, update the `Main.qml` file or add new `.qml` components to the `src/gui` directory.
