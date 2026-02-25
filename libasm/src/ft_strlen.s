section .text
	global ft_strlen

ft_strlen:
	test rdi, rdi
	je skip
	xor rcx, rcx
loop:
	cmp byte [rdi], 0
	je exit_loop
	inc rdi
	add rcx, 1
	jmp loop

exit_loop:
	mov rax, rcx
	ret
skip:
	xor rax, rax
	ret

section .note.GNU-stack noalloc noexec nowrite progbits