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
	.export		_input_vector
	.export		_expected_output_vector
	.export		_coeff
	.export		_filter_state
	.export		_MyFilter
	.export		_main

.segment	"DATA"

_input_vector:
	.word	$0000
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
	.word	$0FFF
_expected_output_vector:
	.word	$0000
	.word	$00FF
	.word	$01FF
	.word	$02FF
	.word	$03FF
	.word	$04FF
	.word	$05FF
	.word	$06FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
	.word	$07FF
_coeff:
	.word	$0800
	.word	$0800
	.word	$0800
	.word	$0800
	.word	$0800
	.word	$0800
	.word	$0800
	.word	$0800
_filter_state:
	.word	$0000
	.word	$0000
	.word	$0000
	.word	$0000
	.word	$0000
	.word	$0000
	.word	$0000
	.word	$0000

; ---------------------------------------------------------------
; int __near__ MyFilter (int)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_MyFilter: near

.segment	"CODE"

;
; {
;
	jsr     pushax
;
; for (ind_taps=8; ind_taps>0; ind_taps--)
;
	jsr     decsp4
	ldx     #$00
	lda     #$08
	ldy     #$02
	jsr     staxysp
L0036:	ldy     #$03
	jsr     ldaxysp
	cmp     #$01
	txa
	sbc     #$00
	bvs     L003D
	eor     #$80
L003D:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jne     L0039
	jmp     L0037
;
; filter_state[ind_taps-1] = filter_state[ind_taps-2];
;
L0039:	ldy     #$03
	jsr     ldaxysp
	jsr     decax1
	jsr     aslax1
	clc
	adc     #<(_filter_state)
	tay
	txa
	adc     #>(_filter_state)
	tax
	tya
	jsr     pushax
	ldy     #$05
	jsr     ldaxysp
	jsr     decax2
	jsr     aslax1
	clc
	adc     #<(_filter_state)
	tay
	txa
	adc     #>(_filter_state)
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	ldy     #$00
	jsr     staxspidx
;
; for (ind_taps=8; ind_taps>0; ind_taps--)
;
	ldy     #$03
	jsr     ldaxysp
	sta     regsave
	stx     regsave+1
	jsr     decax1
	ldy     #$02
	jsr     staxysp
	lda     regsave
	ldx     regsave+1
	jmp     L0036
;
; filter_state[0] = InputVal;
;
L0037:	ldy     #$05
	jsr     ldaxysp
	sta     _filter_state
	stx     _filter_state+1
;
; tmp_val=0;
;
	ldx     #$00
	lda     #$00
	ldy     #$00
	jsr     staxysp
;
; for (ind_taps=0; ind_taps<8; ind_taps++)
;
	ldx     #$00
	lda     #$00
	ldy     #$02
	jsr     staxysp
L0048:	ldy     #$03
	jsr     ldaxysp
	cmp     #$08
	txa
	sbc     #$00
	bvc     L004F
	eor     #$80
L004F:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jne     L004B
	jmp     L0049
;
; tmp_val=tmp_val + (filter_state[ind_taps]*coeff[ind_taps]);
;
L004B:	ldy     #$01
	jsr     ldaxysp
	jsr     pushax
	ldy     #$05
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     #<(_filter_state)
	tay
	txa
	adc     #>(_filter_state)
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	jsr     pushax
	ldy     #$07
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     #<(_coeff)
	tay
	txa
	adc     #>(_coeff)
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	jsr     tosmulax
	jsr     tosaddax
	ldy     #$00
	jsr     staxysp
;
; for (ind_taps=0; ind_taps<8; ind_taps++)
;
	ldy     #$03
	jsr     ldaxysp
	sta     regsave
	stx     regsave+1
	jsr     incax1
	ldy     #$02
	jsr     staxysp
	lda     regsave
	ldx     regsave+1
	jmp     L0048
;
; tmp_val = tmp_val / (4096*8);
;
L0049:	ldy     #$01
	jsr     ldaxysp
	cpx     #$80
	txa
	ldx     #$00
	bcc     L0059
	dex
L0059:	jsr     asrax4
	jsr     asrax3
	ldy     #$00
	jsr     staxysp
;
; return tmp_val;
;
	ldy     #$01
	jsr     ldaxysp
	jmp     L0035
;
; }
;
L0035:	jsr     incsp6
	rts

.endproc

; ---------------------------------------------------------------
; int __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

;
; for (index=0; index<16; index++)
;
	ldy     #$24
	jsr     subysp
	ldx     #$00
	lda     #$00
	ldy     #$22
	jsr     staxysp
L005D:	ldy     #$23
	jsr     ldaxysp
	cmp     #$10
	txa
	sbc     #$00
	bvc     L0064
	eor     #$80
L0064:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jne     L0060
	jmp     L005E
;
; calc_output_vector[index] = MyFilter(input_vector[index]);
;
L0060:	ldy     #$23
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     sp
	tay
	txa
	adc     sp+1
	tax
	tya
	jsr     pushax
	ldy     #$25
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     #<(_input_vector)
	tay
	txa
	adc     #>(_input_vector)
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	jsr     _MyFilter
	ldy     #$00
	jsr     staxspidx
;
; for (index=0; index<16; index++)
;
	ldy     #$23
	jsr     ldaxysp
	sta     regsave
	stx     regsave+1
	jsr     incax1
	ldy     #$22
	jsr     staxysp
	lda     regsave
	ldx     regsave+1
	jmp     L005D
;
; passed = 0xABCD0123;
;
L005E:	ldx     #$01
	lda     #$23
	ldy     #$20
	jsr     staxysp
;
; for (index=0; index<16; index++)
;
	ldx     #$00
	lda     #$00
	ldy     #$22
	jsr     staxysp
L006D:	ldy     #$23
	jsr     ldaxysp
	cmp     #$10
	txa
	sbc     #$00
	bvc     L0074
	eor     #$80
L0074:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jne     L0070
	jmp     L006E
;
; if (calc_output_vector[index] != expected_output_vector[index])
;
L0070:	ldy     #$23
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     sp
	tay
	txa
	adc     sp+1
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	jsr     pushax
	ldy     #$25
	jsr     ldaxysp
	jsr     aslax1
	clc
	adc     #<(_expected_output_vector)
	tay
	txa
	adc     #>(_expected_output_vector)
	tax
	tya
	ldy     #$01
	jsr     ldaxidx
	jsr     tosneax
	jeq     L006F
;
; passed = 0;
;
	ldx     #$00
	lda     #$00
	ldy     #$20
	jsr     staxysp
;
; for (index=0; index<16; index++)
;
L006F:	ldy     #$23
	jsr     ldaxysp
	sta     regsave
	stx     regsave+1
	jsr     incax1
	ldy     #$22
	jsr     staxysp
	lda     regsave
	ldx     regsave+1
	jmp     L006D
;
; if (passed != 0xABCD0123)
;
L006E:	ldy     #$21
	jsr     ldaxysp
	jsr     axlong
	jsr     pusheax
	ldx     #$01
	lda     #$CD
	sta     sreg
	lda     #$AB
	sta     sreg+1
	lda     #$23
	jsr     tosneeax
	jeq     L007C
;
; return 0; /* This statement is unreachable. It is OK. */
;
L007C:	ldx     #$00
	lda     #$00
	jmp     L005B
;
; }
;
L005B:	ldy     #$24
	jsr     addysp
	rts

.endproc
