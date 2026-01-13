section .data
  a dq 10
  b dq 10
  a_msg db 'a '
  o_msg db ' '
  b_msg db ' b', 0x0A
  
  a_msg_len equ $ - a_msg ; tinh tu a_msg toi diem hien tai
  
section .text
global _start

_start:

  call _ftcompare
  
  mov rax, 1
  mov rdi, 1
  mov rsi, a_msg
  mov rdx, a_msg_len
  syscall
  
  mov rax, 60
  mov rdi, 0
  syscall
  
_ftcompare:
  mov r8, [a] ; lay gia tri tai dia chi cua a ghi vao r8
  cmp r8, [b]
  ja _aab
  je _aeb
  mov byte [o_msg], '<'
  ret
  
_aab:
  mov byte [o_msg], '>'
  ret
  
_aeb:
  mov byte [o_msg], '='
  ret
