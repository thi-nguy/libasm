section .text
    extern __errno_location
    global ft_read

ft_read:
    push rbx

    mov rax, 0
    syscall

    test rax, rax
    jns .read_ok

    neg rax
    mov ebx, eax
    call __errno_location wrt ..plt
    mov [rax], ebx
    pop rbx
    mov rax, -1
    ret

.read_ok:
    pop rbx
    ret

section .note.GNU-stack noalloc noexec nowrite progbits