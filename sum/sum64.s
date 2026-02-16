section .rodata
	num1_msg db 'Choose the first number: '
	num1_msg_len equ $ - num1_msg

section .bss
	num1_str resb 1
	num1_str_len resb 1

section .text
	global _start
_start:
	mov rax, 1 ; write
	mov rdi, 1
	mov rsi, num1_msg
	mov rdx, num1_msg_len
	syscall

	mov rax, 0 ; read
	mov rdi, 0
	mov rsi, num1_str
	mov rdx, 10
	syscall 

	 ; Print the number we've just inserted from keyboard.
	; mov rdx, rax ; rax contains the result of the system-call. Here it has the number of read bytes from the step above, hence the length of the first int (in string form)
	; mov rax, 1
	; mov rdi, 1
	; mov rsi, num1_str
	; syscall

	; Convert string to integer
	xor rax, rax ; xor rax rax => gan thanh ghi bang 0 thay vi dung bien cho nhanh
	xor rbx, rbx ; xor rbx rbx
	mov bl, [rsi] ; địa chỉ bộ nhớ, lấy giá trị tại địa chỉ rsi trỏ tới
	cmp bl, 10 ; 10 la ASCII cua \n
	je .read_num1_done

.read_num1_loop:
	sub bl, '0'
	imul rax, rax, 10
	add rax, rbx
	inc rsi
	mov bl, [rsi]
	cmp bl, 10
	jne .read_num1_loop
	
.read_num1_done:
	xor rbx, rbx

	mov rax, 60
	mov rdi, 0
	syscall

	
	
