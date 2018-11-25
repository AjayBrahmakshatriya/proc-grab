#include "asm_offset.h"

	.text
_start:
	jmp start_execution

error_exit:
	movq	%rax, %rdi
	movq	$60, %rax
	syscall
load_section:
	pushq	%rdx
	push	%r8
	leaq	SECTION_filename(%r8), %rdi
	movq	SECTION_open_flags(%r8), %rsi
	movq	$0, %rdx
	movq	$2, %rax
	syscall
	testq	%rax, %rax
	jl	error_exit
	movq	SECTION_start_address(%r8), %rdi
	movq	SECTION_size(%r8), %rsi
	movq	SECTION_proto(%r8), %rdx
	movq	SECTION_flags(%r8), %r10
	movq	SECTION_offset(%r8), %r9
	movq	SECTION_is_raw(%r8), %rcx
	testq	%rcx, %rcx
	jnz 	mmap_raw
	movq	%rax, %r8
	movq	$9, %rax
	syscall
	testq	%rax, %rax
	jl 	error_exit
	jmp	fd_close
mmap_raw:
	movq	$3, %rdx
	movq	%rax, %r12
	movq	$-1, %r8
	movq	$0, %r9
	movq	$9, %rax
	syscall
	test 	%rax, %rax
	jl	error_exit
	movq	%r12, %rdi
	popq	%rcx
	pushq	%rcx
	movq	SECTION_start_address(%rcx), %rsi
	movq	SECTION_size(%rcx), %rdx
	movq	$0, %rax
	syscall
	test	%rax, %rax
	jl	error_exit
	popq	%rcx
	pushq	%rcx
	movq	SECTION_start_address(%rcx), %rdi
	movq	SECTION_size(%rcx), %rsi
	movq	SECTION_proto(%rcx), %rdx
	movq	$10, %rax
	syscall
	testq	%rax, %rax
	jl	error_exit
	movq	%r12, %r8

fd_close:
	movq	%r8, %rdi
	movq	$3, %rax
	syscall
	popq	%r8
	popq	%rdx
	retq


	.globl start_execution
start_execution:
	movq	%rdi, %rbx
	movq	PARAMS_unmap_address_start(%rbx), %rdi
	movq	PARAMS_unmap_address_size(%rbx), %rsi
	movq	$11, %rax
	syscall
	movq	PARAMS_number_of_sections(%rbx), %rdx
	movq	PARAMS_section_data_start(%rbx), %r8
	


repeat_section:
	callq	load_section

	addq	$SECTION_SIZE, %r8
	decq	%rdx
	jnz	repeat_section


loop:
	jmp loop

