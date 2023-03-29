[BITS 64]
GLOBAL Start
EXTERN Execute
[SECTION .text$A]
Start:
	push	rsi
	mov	rsi, rsp
	and	rsp, 0FFFFFFFFFFFFFFF0h
	sub	rsp, 020h
	call	Execute
	mov	rsp, rsi
	pop	rsi
	ret
