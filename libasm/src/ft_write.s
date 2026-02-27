

section .text
    extern __errno_location 
    global ft_write
ft_write:
    push rbx

    mov rax, 1
    syscall

    test rax, rax
    jns .write_ok
    js .error_handler

.write_ok:
    pop rbx
    ret

.error_handler:
    neg rax
    mov ebx, eax
    call __errno_location wrt ..plt
    mov [rax], ebx
    pop rbx
    mov rax, -1
    ret

section .note.GNU-stack noalloc noexec nowrite progbits