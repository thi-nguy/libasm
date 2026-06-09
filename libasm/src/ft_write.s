section .text
    extern __errno_location 
    global ft_write
    
ft_write:
    push rbp
    mov rbp, rsp
    
    push rbx

    and rsp, -16

    mov rax, 1
    syscall

    test rax, rax
    jns .write_ok

    neg rax
    mov ebx, eax
    call __errno_location wrt ..plt
    mov [rax], ebx
    pop rbx
    mov rax, -1

    mov rsp, rbp
    pop rbp
    ret

.write_ok:
    pop rbx

    mov rsp, rbp
    pop rbp
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
