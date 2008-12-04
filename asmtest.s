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
