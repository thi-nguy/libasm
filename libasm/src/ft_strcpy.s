section .text
	global ft_strcpy

ft_strcpy:
    mov rax, rdi
loop:
    mov cl, byte [rsi]
    mov byte[rdi], cl
    inc rsi
    inc rdi
    test cl, cl
    jne loop

    ret

section .note.GNU-stack noalloc noexec nowrite progbits