# simple-edit

A minimal terminal based text editor written in C for Windows :3

This is a learning project to better understanding how text editors work!

---

## Features

- Normal / Insert mode (vim-inspired)
- Edit text in-memory using a fixed-size buffer (for now)
- Open an existing file or create a new one
- Save on exit

---

## Build

Requires:
- GCC (MinGW / MSYS2)
- Windows

Build using make:
```sh
make
```
## Usage

Run with a filename:
```sh
./sedit filename.txt
