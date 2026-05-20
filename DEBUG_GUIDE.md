# Debug Assembly x86_64 using terminal on Linux
## 1. Debug flow (using GDB):
```bash
# Build
nasm -f elf64 -g -F dwarf example.asm -o example.o
ld example.o -o example

# Debug by GDB
gdb ./example

# Inside GDB
(gdb) break _start
(gdb) run
(gdb) layout asm
(gdb) layout regs
(gdb) si              # step instruction
(gdb) info registers
(gdb) x/16xg $rsp     # view stack
```
## 2. Useful commands
### a) View Register

**View all registers:**
```
info registers
```

**View specific Register**
```
info registers rax rbx rcx rdx
info registers rip
```

**View Register in hex:**
```
p/x $rax
p/x $rbx
```

**View Register in decimal:**
```
p/d $rax
```

**View flags:**
```
info registers eflags
```

### b) Memory Viewer (Debug Console)
**View stack:**
```
x/16xg $rsp
```

**View memory at specific address:**
```
x/10xb 0x601000
x/4xw $rsi
```

Format:
- `x` = examine
- `16` = số lượng
- `x` = hex format (or: d-decimal, u-unsigned, t-binary)
- `g` = giant (8 bytes), `w` = word (4 bytes), `h` = half (2 bytes), `b` = byte

**View memory:**
```
x/s $rsi          # View string
x/16xg $rsp       # View stack (16 quadwords)
x/4xw numbers     # View array
```

### c) Display auto
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

### b) Watchpoint (stop when variables change values)
```
watch result
```

### d) View source code + assembly together
```
layout split
```