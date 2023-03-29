[BITS 32]
GLOBAL _Start
EXTERN _Execute
[SECTION .text$A]
_Start:
	push	ebp
	mov	ebp, esp
	call	_Execute
	mov	esp, ebp
	pop	ebp
	ret
