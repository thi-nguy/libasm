section .text
	global ft_sum

ft_sum:
	mov rax, rdi
	add rax, rsi
	ret

section .note.GNU-stack noalloc noexec nowrite progbits