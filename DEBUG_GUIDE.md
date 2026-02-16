# Hướng dẫn Debug Assembly x86_64 trong terminal trên Linux

## 1. Cài đặt công cụ cần thiết

### Cài đặt NASM và GDB
```bash
sudo apt update
sudo apt install nasm gdb build-essential
```

## 2. Build và Debug

### Build bằng keyboard shortcut
- **make**: Build file hiện tại

### Bắt đầu Debug
- **gdb ./program_name**: Start debugging

## 3. Quan sát thanh ghi

### a) Debug Console
Trong Debug Console, bạn có thể gõ các lệnh GDB:

**Xem tất cả thanh ghi:**
```
info registers
```

**Xem thanh ghi cụ thể:**
```
info registers rax rbx rcx rdx
info registers rip
```

**Xem thanh ghi dạng hex:**
```
p/x $rax
p/x $rbx
```

**Xem thanh ghi dạng decimal:**
```
p/d $rax
```

**Xem flags:**
```
info registers eflags
```

### b) Watch Window
1. Mở: View → Run → Watch (hoặc Ctrl+Shift+D)
2. Click dấu "+" để thêm expression
3. Thêm các thanh ghi muốn theo dõi:
   - `$rax`
   - `$rbx`
   - `$rcx`
   - `$rsp`
   - `$rip`
   - `$eflags`

### c) Variables Window
- Tự động hiển thị các biến và thanh ghi
- Có thể expand để xem chi tiết

### d) Memory Viewer (Debug Console)
**Xem stack:**
```
x/16xg $rsp
```

**Xem memory tại địa chỉ cụ thể:**
```
x/10xb 0x601000
x/4xw $rsi
```

Format:
- `x` = examine
- `16` = số lượng
- `x` = hex format (có thể dùng: d-decimal, u-unsigned, t-binary)
- `g` = giant (8 bytes), `w` = word (4 bytes), `h` = half (2 bytes), `b` = byte

## 5. Các phím tắt Debug

- **F5**: Continue / Start
- **F9**: Toggle breakpoint
- **F10**: Step Over (next instruction, không vào hàm)
- **F11**: Step Into (step instruction, vào hàm nếu gọi)
- **Shift+F11**: Step Out
- **Ctrl+Shift+F5**: Restart
- **Shift+F5**: Stop

## 6. Lệnh GDB hữu ích trong Debug Console

```bash
# Disassemble
disassemble _start
disas

# Xem instruction tiếp theo
x/5i $rip

# Set breakpoint
break *0x401000
break _start

# Xem stack frame
info frame
info stack

# Examine memory
x/s $rsi          # Xem string
x/16xg $rsp       # Xem stack (16 quadwords)
x/4xw numbers     # Xem array

# Display auto (hiển thị mỗi lần dừng)
display/x $rax
display/i $rip

# Undisplay
undisplay 1

# Custom commands (nếu dùng .gdbinit)
regs              # Show all registers formatted
flags             # Show CPU flags
stack             # Show stack contents
```

## 7. Tips & Tricks

### a) Tự động hiển thị thanh ghi
Thêm vào Debug Console:
```
display/x $rax
display/x $rbx
display/x $rcx
display/x $rdx
```

### b) Breakpoint conditional
```
break _start if $rax == 0x10
```

### c) Watchpoint (dừng khi biến thay đổi)
```
watch result
```

### d) Xem source code + assembly cùng lúc
```
layout split
```

### e) TUI Mode trong terminal GDB
```bash
# Chạy GDB trực tiếp trong terminal (không qua VSCode)
gdb ./example
(gdb) layout asm
(gdb) layout regs
(gdb) break _start
(gdb) run
(gdb) si    # step instruction
```

## 8. Debug từ Command Line (Alternative)

Nếu muốn debug trực tiếp bằng GDB trong terminal:

```bash
# Build
nasm -f elf64 -g -F dwarf example.asm -o example.o
ld example.o -o example

# Debug
gdb ./example

# Trong GDB
(gdb) break _start
(gdb) run
(gdb) layout asm
(gdb) layout regs
(gdb) si              # step instruction
(gdb) info registers
(gdb) x/16xg $rsp     # xem stack
```

## 9. Troubleshooting

### Lỗi: "No debugging symbols found"
- Đảm bảo build với flags `-g -F dwarf`

### Lỗi: "Cannot find bounds of current function"
- Normal với assembly, có thể bỏ qua

### VSCode không dừng tại breakpoint
- Kiểm tra file đã được build với debug symbols
- Thử set breakpoint bằng địa chỉ: `break *0x401000`

### Không thấy thanh ghi trong Variables window
- Dùng Debug Console với lệnh `info registers`
- Hoặc thêm vào Watch window: `$rax`, `$rbx`, etc.

## 10. Ví dụ Debug Session

1. Mở file `example.asm` trong VSCode
2. Nhấn **F5** để bắt đầu debug
3. Chương trình dừng tại `_start`
4. Mở Watch window, thêm: `$rax`, `$rbx`, `$rcx`, `$rsi`
5. Nhấn **F10** để step qua từng instruction
6. Quan sát giá trị thanh ghi thay đổi
7. Trong Debug Console, gõ: `x/5xw numbers` để xem array
8. Tiếp tục step cho đến khi vào loop
9. Quan sát `$rcx` giảm dần và `$rax` tăng lên

