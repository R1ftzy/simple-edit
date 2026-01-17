# simple-edit

A minimal terminal based text editor written in C for Windows :3

This is a learning project to better understand how text editors work! (also learning how to use GitHub >w<)

**[Download the latest Windows build!!](https://github.com/R1ftzy/simple-edit/releases/latest)**

---

## Features

- Normal / Insert mode (vim-inspired)
- Cursor support with arrow key movement
- Edit text in-memory using a fixed-size buffer (for now)
- Open existing files or create new ones
- Explicit save and quit commands with unsaved changes protection

---

## Build
Requires:
- GCC (MinGW / MSYS2)
- Windows

Build using make:
```sh
make            # Release build (optimized)
make dev        # Development build (with debug symbols)
```

---

## Usage
#### Adding to PATH (recommended):
1. Copy `sedit.exe` to a folder (e.g., `C:\Tools\`)
2. Search "Environment Variables" in Start Menu
3. Edit "Path" and add your folder
4. Restart your terminal

Then run from anywhere:
```sh
sedit filename.txt
```

Or run directly from the folder without PATH setup:
```sh
./sedit filename.txt
```
---
## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and changes.




