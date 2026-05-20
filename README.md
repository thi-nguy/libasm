# x86-64 Assembly Learning Journey & libasm

A comprehensive repository documenting my research into **x86-64** computer architecture and hands-on practice with **Assembly (Intel Syntax)** programming. This project features foundational exercises and a library of C functions entirely rewritten in Assembly.

---

## 📌 Overview

The core objective of this repository is to gain proficiency in low-level programming by deeply understanding memory management, registers, and 64-bit calling conventions.

The focal point is **`libasm`** — a personal library that recreates standard C functions, optimized and written entirely in x86-64 Assembly.

---

## 📂 Repository Structure

| Directory / File | Description |
| :--- | :--- |
| `📂 libasm/` | The main directory containing the Assembly library source code (`strlen`, `strcpy`, etc.). |
| `📂 hello/` | A "Hello World" starter program to set up the environment and test the compiler. |
| `📂 if_else/` | Practice exercises covering conditional statements and branching (`jmp`, `je`, `jne`, etc.). |
| `📂 sum/` | Basic arithmetic programs to practice logic and mathematical operations. |
| `📄 DEBUG_GUIDE.md` | A guide for debugging Assembly code (GDB/LLDB) and handling Segmentation Faults. |

---

## 🛠 The `libasm` Library

The `libasm` directory contains functions rewritten to explore the internal mechanics of `libc`:

* **String Manipulation:**
    * `ft_strlen` (Recreates `strlen`) – Counts string length.
    * `ft_strcpy` (Recreates `strcpy`) – Copies a string.
    * `ft_strcmp` (Recreates `strcmp`) – Compares two strings.
    * `ft_strdup` (Recreates `strdup`) – Duplicates a string (utilizes `malloc`).
* **System I/O (System Calls):**
    * `ft_write` (Recreates `write`) – Writes data to a file descriptor.
    * `ft_read` (Recreates `read`) – Reads data from a file descriptor.

### Prerequisites
* **Architecture:** x86-64
* **Syntax:** Intel Syntax
* **Assembler:** `NASM` (Netwide Assembler)
* **Compiler:** `gcc` or `clang` (to link with C test files)

---