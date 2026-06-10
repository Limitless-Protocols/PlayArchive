# PlayArchive GUI User Guide

The PlayArchive Graphical User Interface is built with Qt Quick/QML, designed for high-performance and a modern gamer aesthetic.

## Interface Overview

### 1. Toolbar
Located at the top, the toolbar provides quick access to core functions:
- **Open**: Opens a file dialog to select an existing archive (`.gcf`, `.zip`, `.7z`, `.tar`).
- **Extract**: Becomes active when an archive is loaded. Allows you to select a destination folder for extraction.
- **Settings**: Access application preferences (Theme, Performance, Presets).

### 2. Archive Browser
The central area displays the contents of the currently loaded archive:
- **Path**: The relative path of the file or directory within the archive.
- **Size**: The uncompressed size of the asset.
- **Icon Indicators**: Visual cues for files vs. directories.

### 3. Status Bar
Located at the bottom:
- **Status Text**: Displays current activity (e.g., "Loaded 150 entries", "Extracting: texture_01.png").
- **Progress Bar**: Shows real-time progress for compression and extraction tasks.

## Common Workflows

### Opening an Archive
You can open an archive in two ways:
1. Click the **Open** button in the toolbar and select your file.
2. **Drag and Drop**: Drag an archive file from your OS file explorer directly into the PlayArchive window.

### Creating a GCF Archive
To create a new game-optimized archive:
1. (In future versions) Select "New Archive".
2. Drag the folders you wish to compress into the window.
3. Select a preset:
    - **Fast Pack**: Minimum compression, maximum speed.
    - **Game Folder Optimization**: Balanced Zstd compression with asset-aware skipping.
    - **Maximum Compression**: High Zstd levels for smallest file size.

### Extracting
1. Open an archive.
2. Click **Extract**.
3. Choose a destination folder and wait for the progress bar to complete.

---
*Developed by Limitless Protocols*
