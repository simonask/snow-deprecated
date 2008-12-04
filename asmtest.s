.globl _simple_loop
_simple_loop:
	enter $0, $0
	xor %rax, %rax
	mov $200, %rbx
loop_cond:
	cmp %rax, %rbx
	jz loop_exit
	inc %rax
	jmp loop_cond
loop_exit:
	leave
	ret

.globl _movregreg
_movregreg:
	mov %rax, %rbx
	mov %rax, %r9
	mov %r9, %rax
	ret

.globl _pushpopmem
_pushpopmem:
	push (%rsp)
	pop %rax
	ret
.globl _pushpop
_pushpop:
	push %r9
	pop %r9
	ret
	
.globl _end
_end:
