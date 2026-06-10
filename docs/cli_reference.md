# CLI Reference

`playarchive` is the command-line interface for the PlayArchive engine.

## Usage

```bash
playarchive <command> <archive_path> [arguments]
```

## Commands

### `create`
Compresses files or folders into a new archive.

- **Syntax**: `playarchive create <archive_path> <source1> [source2 ...]`
- **Example**: `playarchive create backup.gcf ./docs ./photos`
- **Supported Outputs**: `.gcf`, `.zip`, `.7z`, `.tar`.

### `extract`
Extracts the contents of an archive to a destination folder.

- **Syntax**: `playarchive extract <archive_path> <destination_folder>`
- **Example**: `playarchive extract game_assets.gcf ./dist`

### `list`
Lists all files and directories inside an archive.

- **Syntax**: `playarchive list <archive_path>`
- **Example**: `playarchive list data.zip`

## Global Options
- **Format Detection**: The tool automatically selects the appropriate compression engine based on the file extension of the `<archive_path>`.
- **Progress Tracking**: A real-time progress bar is displayed in the terminal during creation and extraction.
