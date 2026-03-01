section .text
    extern ft_strlen
    extern malloc 
    global ft_strdup

ft_strdup:
    push rbx
    mov rbx, rdi
    call ft_strlen
    add rax, 1
    mov rdi, rax
    call malloc wrt ..plt
    test rax, rax
    jnz .copy_step

    mov rax, 0
    pop rbx
    ret

.copy_step:
    push rbp
    mov rbp, rax

.loop:
    mov cl, byte [rbx]
    mov byte [rax], cl
    cmp byte [rbx], 0
    je .exit_loop
    inc rbx
    inc rax
    jmp .loop

.exit_loop:

    mov rax, rbp
    pop rbp
    pop rbx
    ret

section .note.GNU-stack noalloc noexec nowrite progbits