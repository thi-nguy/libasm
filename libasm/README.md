# Libasm Project

This project contains a collection of functions written in x86-64 Assembly, compiled into a static library (`libasm.a`).

## System Requirements

- **C Compiler:** `gcc`
- **Assembly Compiler:** `nasm`
- **OS Compatibility:**
  - The configuration is set for Linux x86-64 by default.
  - If using macOS, change `NASM_FLAGS` in the Makefile to `macho64 -Fmacho` to ensure compatibility with `gdb`.

## Directory Structure

- `src/`: Contains source code files (`.s`).
- `include/`: Contains header files.
- `obj/`: Contains object files generated during compilation.
- `lib/`: Contains the generated static library `libasm.a`.

## Usage

Use the following commands in your terminal to manage the project:

| Command              | Description                                            |
| :------------------- | :----------------------------------------------------- |
| `make` or `make all` | Compiles the library and the test program.             |
| `make test`          | Runs the test program to verify the library functions. |
| `make clean`         | Removes object files.                                  |
| `make fclean`        | Removes object files, the library, and executables.    |
| `make re`            | Cleans everything and rebuilds the project.            |

## Technical Details

- **Archiving:** The static library is created using `ar` with the flags `rcs` (r=insert, c=create, s=add symbol index).
- **Assembly Compilation:** Assembly source files are compiled using the `elf64` format with debug information enabled (`-g -Fdwarf`).
- **Linking:** The test program is linked against the library using the `-lasm` flag.

---

_Note: If you are working on macOS, please remember to update the `NASM_FLAGS` in your `Makefile` before running the build commands._
