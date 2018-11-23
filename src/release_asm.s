	.text
	.globl jump_to_raw_binary
jump_to_raw_binary:
	jmpq *%rdi
