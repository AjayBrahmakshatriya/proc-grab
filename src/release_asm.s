	.text
	.globl jump_to_raw_binary
jump_to_raw_binary:
	movq	%rdi, %rax
	movq	%rsi, %rdi
	movq	%rdx, %rsp
	jmpq 	*%rax

