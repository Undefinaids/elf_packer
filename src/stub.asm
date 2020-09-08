[BITS 64]

global stub:function
global stub_size:data

stub_size dq end - stub

stub:
	mov	rdi, 1
	mov rax, 60
	syscall
	ret
end: