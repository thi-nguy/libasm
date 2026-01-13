section .rodata
	hello_msg db 'Hello World', 0x0A
	hello_msg_len equ $ - hello_msg

section .text
	global _start
_start:
	mov rax, 1
	mov rdi, 1
	mov rsi, hello_msg
	mov rdx, hello_msg_len
	syscall

	mov rax, 60	
	mov rdi, 0
	syscall
