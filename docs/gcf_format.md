# GCF (Game Compression Format) Specification

The `.gcf` format is specifically designed to optimize the storage and distribution of game assets.

## Archive Structure

A GCF file consists of three main sections:

1.  **Header**: Contains the magic number, version, and the offset to the Entry Table.
2.  **Data Blocks**: The actual content of the files, either raw or compressed.
3.  **Entry Table**: A list of all files in the archive, located at the end of the file for easier appending/modification.

### 1. Header (16 bytes)
| Field | Type | Size | Description |
| :--- | :--- | :--- | :--- |
| Magic | char[4] | 4 | 'GCF1' |
| Entry Table Offset | uint64_t | 8 | Absolute offset to the start of the Entry Table |
| Entry Count | uint32_t | 4 | Total number of files in the archive |

### 2. Entry Table
Each entry in the table describes a single file:
- **Path Length** (uint32_t)
- **Path String** (char[])
- **Original Size** (uint64_t)
- **Compressed Size** (uint64_t)
- **Data Offset** (uint64_t) - Absolute offset to the start of the data block.
- **Is Compressed** (bool)

## Game-Specific Optimizations

### Asset-Aware Preprocessing
GCF detects files that are already compressed (e.g., `.png`, `.jpg`, `.mp3`, `.ogg`, `.mp4`) and stores them in "Raw" mode. This avoids the CPU overhead of attempting to re-compress data that won't shrink further, significantly speeding up the archiving process.

### Compression Engine
GCF uses **Zstandard (Zstd)**, which provides compression ratios comparable to Deflate but with significantly faster decompression speeds, making it ideal for games where loading times are critical.

### Deduplication (Planned)
The format supports content-addressable storage. Identical assets across different folders (common in game updates) are stored once and referenced multiple times in the Entry Table.
