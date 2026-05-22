section .text
    extern __errno_location
    global ft_read

ft_read:
    push rbp
    mov rbp, rsp
    and rsp, -16
    
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

    mov rsp, rbp
    pop rbp
    ret

.read_ok:
    pop rbx

    mov rsp, rbp
    pop rbp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits