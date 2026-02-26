section .text
    global ft_strcmp
ft_strcmp:

loop:
    mov cl, byte [rdi]
    test cl, cl
    je exit_loop
    cmp cl, byte [rsi]
    jne exit_loop
    
    inc rdi
    inc rsi
    jmp loop

exit_loop:
    movzx rax, cl
    movzx rbx, byte [rsi]
    sub rax, rbx
    ret

section .note.GNU-stack noalloc noexec nowrite progbits