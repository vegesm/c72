/* Macros that generate PDP-11 like movb instruction */

.macro __isreg name ind
    .set _reg\ind, 0
    .ifeqs "\name", "%eax"
    .set _reg\ind, 1
    .endif

    .ifeqs "\name", "%ebx"
    .set _reg\ind, 1
    .endif

    .ifeqs "\name", "%ecx"
    .set _reg\ind, 1
    .endif

    .ifeqs "\name", "%edx"
    .set _reg\ind, 1
    .endif

    .ifeqs "\name", "%esi"
    .set _reg\ind, 1
    .endif

    .ifeqs "\name", "%edi"
    .set _reg\ind, 1
    .endif
.endm

.macro __prinst instr, arg1, arg2
    .ifeqs "\arg1", "%eax"
      \instr %al, \arg2
    .else
      .ifeqs "\arg1", "%ebx"
        \instr %bl, \arg2
      .else
        .ifeqs "\arg1", "%ecx"
          \instr %cl, \arg2
        .else
          .ifeqs "\arg1", "%edx"
            \instr %dl, \arg2
          .else
            \instr \arg1, \arg2
          .endif
        .endif
      .endif
    .endif
.endm

.macro c72_mov size=l, from, to
.ifeqs "\size", "l"
.ifnes "\from","\to"
movl \from, \to
.endif
.else

__isreg \to, 2

.if _reg2
__prinst movsbl, \from, \to
.else
__prinst movb, \from, \to
.endif

.endif
.endm

/* eax contains the value we want to switch on, ebx has the switch table address */
1:
	addl $8, %ebx
bswitch:
2:	cmp	(%ebx),%eax     /* if first column matches, check second col */
	je	1f
	testl	$-1, 4(%ebx)
	jnz	1b
2:
	jmp	*(%ebx)
1:
	testl	$-1, 4(%ebx)
	jz 2b
	jmp	*4(%ebx)
