# CLI Reference

`playarchive-cli` is the command-line interface for the PlayArchive engine.

## Usage

```bash
playarchive-cli <command> <archive_path> [arguments]
```

## Commands

### `create`
Compresses files or folders into a new archive.

- **Syntax**: `playarchive-cli create <archive_path> <source1> [source2 ...]`
- **Example**: `playarchive-cli create backup.gcf ./docs ./photos`
- **Supported Outputs**: `.gcf`, `.zip`, `.7z`, `.tar`.

### `extract`
Extracts the contents of an archive to a destination folder.

- **Syntax**: `playarchive-cli extract <archive_path> <destination_folder>`
- **Example**: `playarchive-cli extract game_assets.gcf ./dist`

### `list`
Lists all files and directories inside an archive.

- **Syntax**: `playarchive-cli list <archive_path>`
- **Example**: `playarchive-cli list data.zip`

## Global Options
- **Format Detection**: The tool automatically selects the appropriate compression engine based on the file extension of the `<archive_path>`.
- **Progress Tracking**: A real-time progress bar is displayed in the terminal during creation and extraction.
