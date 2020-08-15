[BITS 64]

global stub:function
global stub_size:data

stub_size dq end - stub

stub:
	push rbp
	mov rbp, rsp
	mov rax, 10
	leave
	ret
end: