# Architectural Overview

PlayArchive is designed with a clear separation between its high-performance compression core and its user interfaces.

## High-Level Architecture

The project is split into three main layers:

### 1. Core Engine (`src/core`)
The heart of PlayArchive. It provides a unified interface (`IArchiveHandler`) for all archive operations.
- **Factory Pattern**: `FormatManager` acts as a factory/registry, mapping file extensions to the appropriate handler.
- **Abstraction**: `IArchiveHandler` ensures that the GUI and CLI don't need to know the specifics of whether they are handling a `.zip` via `libarchive` or a `.gcf` via the custom engine.
- **Hybrid Approach**: We use `libarchive` for industry-standard formats to ensure compatibility, while the `GCFEngine` handles our proprietary optimized format.

### 2. Backend Bridge (`src/gui/ArchiveModel.cpp`)
Connects the C++ core to the QML frontend.
- **Asynchrony**: All heavy operations (compression/extraction) are run on background threads using `QtConcurrent` to prevent UI freezing.
- **Progress Reporting**: Uses a callback system within the core that triggers thread-safe UI updates via `QMetaObject::invokeMethod`.

### 3. Frontend / User Interface
- **GUI (`src/gui`)**: A Qt Quick / QML application offering a modern, hardware-accelerated interface.
- **CLI (`src/cli`)**: A lightweight wrapper around the `FormatManager` for terminal usage.

## Design Principles
- **Performance First**: Minimal data copying, multi-threaded operations.
- **Extensibility**: Adding a new format only requires implementing `IArchiveHandler` and registering it in `FormatManager`.
- **Portability**: Uses standard C++17 and cross-platform libraries (Qt, libarchive, Zstd).
