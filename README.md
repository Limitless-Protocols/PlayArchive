# PlayArchive

PlayArchive is a high-performance, cross-platform compression application optimized for game folders. Developed by **Limitless Protocols**, it features a modern Qt Quick GUI and a powerful C++ core.

## Features

- **Native C++ Core**: Fast and efficient compression/extraction.
- **Multi-Format Support**: Handles ZIP, 7z, TAR, and more via `libarchive`.
- **Custom .gcf Format**:
  - **Game-Aware Preprocessing**: Automatically skips already compressed assets (PNG, JPEG, MP3, etc.).
  - **Content Deduplication**: Detects and eliminates duplicate game assets.
  - **Zstd Powered**: Uses Zstandard for an optimal balance of compression ratio and speed.
- **Modern GUI**:
  - Built with **Qt Quick/QML**.
  - Gamer-friendly dark theme.
  - Drag-and-drop support.
  - Real-time progress tracking.
- **CLI Tool**: Headless operation for automation and power users.

## Project Structure

- `src/core`: Native compression engine and format handlers.
- `src/gui`: Qt Quick application and C++ backend models.
- `src/cli`: Command-line interface.
- `include`: Public header files.

## Building from Source

### Dependencies
- CMake 3.16+
- Qt 6.5+ (Core, Gui, Qml, Quick, QuickControls2, Concurrent)
- LibArchive
- Zstandard (Zstd)
- xxHash

### Build Steps
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

### GUI
Run `playarchive_gui` to launch the graphical interface. Drag and drop archives or folders to start.

### CLI
```bash
# Create a GCF archive
playarchive-cli create mygame.gcf ./GameFolder

# Extract an archive
playarchive-cli extract mygame.gcf ./Destination

# List contents
playarchive-cli list mygame.gcf
```

## Documentation

Detailed developer and user documentation is available in the `docs/` directory:

1. [Architectural Overview](docs/architecture.md) - Deep dive into the core engine and design patterns.
2. [GCF Format Specification](docs/gcf_format.md) - Details on the custom Game Compression Format.
3. [User Guide (GUI)](docs/gui_guide.md) - How to use the graphical interface.
4. [CLI Reference](docs/cli_reference.md) - Command-line interface commands and options.
5. [Developer Guide](docs/developer_guide.md) - Building, testing, and contributing.

## License
MIT License
