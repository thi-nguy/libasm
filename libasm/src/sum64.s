section .text
	global _ft_sum
_ft_sum:
	mov rax, rdi
	add rax, rsi
	ret