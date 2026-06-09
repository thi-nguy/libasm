section .text
    extern ft_strlen
    extern malloc 
    extern __errno_location
    global ft_strdup

ft_strdup:
    push rbp
    mov rbp, rsp
    and rsp, -16

    push rbx

    mov rbx, rdi
    call ft_strlen
    add rax, 1
    mov rdi, rax
    call malloc wrt ..plt
    test rax, rax
    jz .handle_error
    jnz .copy_step

.handle_error:
    call __errno_location wrt ..plt
    mov dword [rax], 12
    mov rax, 0
    pop rbx

    mov rsp, rbp
    pop rbp
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

    mov rsp, rbp
    pop rbp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
