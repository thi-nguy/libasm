# Debug Assembly x86_64 trong terminal trên Linux
## 1. Flow debug bằng GDB trong terminal:

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
## 2. Các lệnh hữu ích
### a) Xem thanh ghi

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

### b) Memory Viewer (Debug Console)
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

**Xem memory:**
```
x/s $rsi          # Xem string
x/16xg $rsp       # Xem stack (16 quadwords)
x/4xw numbers     # Xem array
```

### c) Display auto (hiển thị mỗi lần dừng)
```
display/x $rax
display/i $rip
```
**Undisplay**
```
undisplay 1
```

## 3. Tips & Tricks

### a) Breakpoint conditional
```
break _start if $rax == 0x10
```

### b) Watchpoint (dừng khi biến thay đổi)
```
watch result
```

### d) Xem source code + assembly cùng lúc
```
layout split
```