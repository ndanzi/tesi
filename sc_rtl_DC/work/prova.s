;
; File generated by cc65 v 2.15 - Git 377f31d
;
	.fopt		compiler,"cc65 v 2.15 - Git 377f31d"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.forceimport	__STARTUP__
	.export		_main

; ---------------------------------------------------------------
; int __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

;
; int passed = 0xABCD0123;
;
	ldx     #$01
	lda     #$23
	jsr     pushax
;
; return 0;
;
	ldx     #$00
	lda     #$00
	jmp     L0001
;
; }
;
L0001:	jsr     incsp2
	rts

.endproc

