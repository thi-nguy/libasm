section .data
  a dq 10
  b dq 10
  a_msg db 'a '
  o_msg db ' '
  b_msg db ' b', 0x0A
  
  a_msg_len equ $ - a_msg ;get length from a_msg until the current position  
  
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
  
_ftcompare: ;_label: code does not run 
  mov r8, [a] ;get the value from address stored at a into register r8 
  cmp r8, [b]
  ja _aab ;aab = a above b
  je _aeb ;aeb = a equal b
  mov byte [o_msg], '<'
  ret
  
_aab:
  mov byte [o_msg], '>'
  ret
  
_aeb:
  mov byte [o_msg], '='
  ret
