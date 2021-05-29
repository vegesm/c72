/* c code tables-- compile to register */

fp = 1		/* enable floating-point */

.globl	_regtab

_regtab=.;.int .+4
	.int 20, cr20
	.int 21, cr20
	.int 22, cr20
	.int 30, cr30
	.int 31, cr30
	.int 32, cr32
	.int 33, cr32
	.int 34, cr34
	.int 35, cr35
	.int 29, cr29
	.int 36, cr36
	.int 37, cr37
	.int 38, cr38
	.int 101, cr100
	.int 80, cr80
	.int 40, cr40
	.int 41, cr40	/* - like + */
	.int 42, cr42
	.int 43, cr43
	.int 44, cr43
	.int 45, cr45
	.int 46, cr45
	.int 47, cr48
	.int 48, cr48
	.int 60, cr60
	.int 61, cr60
	.int 62, cr60
	.int 63, cr60
	.int 64, cr60
	.int 65, cr60
	.int 66, cr60
	.int 67, cr60
	.int 68, cr60
	.int 69, cr60
	.int 70, cr70
	.int 71, cr70
	.int 72, cr72
	.int 73, cr73
	.int 74, cr73
	.int 75, cr75
	.int 76, cr75
	.int 77, cr70
	.int 78, cr70
	.int 102, cr102
	.int 97, cr97
	.int 0

/* goto */
cr102:
%i,n
	jmp	*A1

%n*,n
	F*
	jmp	*#1(R)

%n,n
	F
	jmp	(R)

/* call */
cr100:
%n*,n
	F*
	call	#1(R)

%a,n
	call	A1

%n,n
	F
	call	R


/* name, constant
   if arg1 is constant, its type is int (constant are always ints at this point)
   otherwise the argument is addressible, which is mem in x86 => we can use movsbl
 */
cr20:
%z,n
	xor	R,R

%aw,n
	mov	A,R

%ab,n
	movsbl	A,R

/*++,-- prefix */
cr30:
%ai,n
%abp,n
%ab,n
	IB1	A1
	c72_mov	B1, A1,R

%a,n
	I'	$4,A1
	mov	A1,R

%nbp*,n
%ni*,n
%nb*,n
	F*
	IB1	#1(R)
	c72_mov	B1, #1(R),R

%n*,n
	F*
	I'	$4,#1(R)
	mov	#1(R),R

/* ++,-- postfix */
cr32:
%ai,n
%abp,n
%ab,n
	c72_mov	B1, A1,R
	IB1	A1

%a,n
	c72_mov	A1,R
	I'	$4,A1

%nb*,n
	F*
	push	#1(R)
	Ib	#1(R)
	pop	R
	c72_mov	b,R,R

%ni*,n
%nbp*,n
	F*
	push	#1(R)
	IB1	#1(R)
	pop	R

%n*,n
	F*
	push	#1(R)
	I'	$4,#1(R)
	pop	R

/* !
 dl is the last register, so it is safe to modify it
*/
cr34:
%n,n
	FC
	setz	%dl
	movzbl  %dl,R

/* &unary */
cr35:
%a,n
	mov	$A1,R

/* & unary of auto */
cr29:
%e,n
	lea	Z(%ebp),R

/* *unary */
cr36:
%nbp,n
	H*
	movsbl	~(R),R

%n,n
	H*
	mov	~(R),R

/* - unary */
cr37:
%n,n
	F
	neg	R

/* ~ (unimplemented) */
cr38:
%n,n
	F
	not	R

/* = */
cr80:
%a,n
	S
	c72_mov	B1, R,A1

%n*,c
	F*
	c72_mov	B1, A2,#1(R)
	c72_mov	B1, #1(R),R

%n*,e
	F*
	S1
	c72_mov	B1, R1,#1(R)
	mov	R1,R

%n*,n
	FS*
	S
	pop %edi
	c72_mov	B1, R,(%edi)

/* | and & */
cr48:
%n,aw
	F
	I	A2,R

%n,ew*
	F
	S1*
	I	#2(R1),R

%n,e
	F
	S1
	I	R1,R

%n,n
	FS
	S
	pop %edi
	I	%edi,R

/* relationals */
cr60:
%n,n
	HC
	setI	%dl
	movzbl	%dl, R

/* >>, << */
cr45:
%n,c
	F
	I	A2,R
	c72_mov	B1, R, R

%n,n
	?2push	%cl
	FS
	S
	c72_mov	b, R, %cl
	Il	%cl, (%esp)
	pop	R
	c72_mov	B1, R, R
	?2pop	%cl

/* +, - */
cr40:
%n,aw
	F
	I	A2,R

%n,ew*
	F
	S1*
	I	#2(R1),R

%n,e
	F
	S1
	I	R1,R

%n,nw*
	SS*
	F
	pop	%edi
	I	(%edi),R

%n,n
	SS
	F
	pop	%edi
	I	%edi,R

/* * */
cr42:
%aw,a
	c72_mov B2, A2,R
	imul	A1,R

%n,aw
	F
	imul	A2,R

%n,e
	F
	S1
	imul	R1,R

%n,n
	FS
	S
	pop	%edi
	imul	%edi,R

/* /; mod
   The second argument is calculated first then moved to edi.
   This is needed because eax and edx are used for the dividend.
   Only eax has to be saved, it is safe to destroy edx as it is the last register.
*/
cr43:
%a,c
%a,ab
	c72_mov	B2, A2, %edi
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idiv	%edi
	mov	I,R
	?0pop	%eax

%a,aw
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idivl	A2
	mov	I,R
	?0pop	%eax

%a,n
	S
	mov	R, %edi
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idiv	%edi
	mov	I,R
	?0pop	%eax

%n,c
%n,ab
	?0push	%eax
	F
	?0mov	R, %eax
	cdq
	c72_mov B2, A2, %edi
	idiv	%edi
	mov	I,R
	?0pop	%eax

%n,aw
	?0push	%eax
	F
	?0mov	R, %eax
	cdq
	idiv	A2
	mov	I,R
	?0pop	%eax

%n,n
    ?0push	%eax
	SS
	F
	?0mov	R, %eax
	cdq
	pop %edi
	idiv	%edi
	mov	I,R
	?0pop	%eax

/* =* */
cr72:
%aw,cw
	imul	A2,A1,R
	movl	R, A1

%aw,n
	S
	imul	A1,R
	movl	R, A1

%a,n
	S
	c72_mov B1, A1, %edi
	imul	%edi,R
	c72_mov	B1, R, A1
	c72_mov	B1, R, R

%ew*,n
	S
	F1*
	imul	#1(R1),R
	mov	R, #1(R1)

/* only active for byte arg1 */
%e*,n
	S
	F1*
	c72_mov B1, #1(R1), %edi
	imul	%edi,R
	c72_mov	B1, R, #1(R1)
	c72_mov	B1, R, R

%n*,n
	FS*
	S
	pop	%edi
	c72_mov B1, (%edi),%esi
	imul	%esi,R
	c72_mov	B1,R,(%edi)
	c72_mov	B1, R, R

/* =mod, =/ */
cr73:
%a,c
%a,ab
	c72_mov	B2, A2, %edi
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idiv	%edi
	c72_mov	B1,I,A1
	c72_mov	B1,I,R
	?0pop	%eax

%a,aw
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idivl	A2
	c72_mov	B1,I,A1
	c72_mov	B1,I,R
	?0pop	%eax

%a,n
	S
	mov	R, %edi
	?0push	%eax
	c72_mov	B1, A1, %eax
	cdq
	idiv	%edi
	c72_mov	B1,I,A1
	c72_mov	B1,I,R
	?0pop	%eax

%n*,c
%n*,ab
	?0push	%eax
	F*
	mov	R, %esi
	c72_mov	B1,#1(%esi), %eax
	cdq
	c72_mov B2, A2, %edi
	idiv	%edi
	c72_mov	B1,I,#1(%esi)
	c72_mov	B1,I,R
	?0pop	%eax

%n*,aw
	?0push	%eax
	F*
	mov	R, %esi
	c72_mov	B1,#1(%esi), %eax
	cdq
	idiv	A2
	c72_mov	B1,I,#1(%esi)
	c72_mov	B1,I,R
	?0pop	%eax

%n*,n
	?0push	%eax
	SS
	F*
	mov	R, %esi
	c72_mov	B1,#1(%esi), %eax
	cdq
	pop %edi
	idiv	%edi
	c72_mov	B1,I,#1(%esi)
	c72_mov	B1,I,R
	?0pop	%eax


/* =>>, =<< */
cr75:
%a,c
	F
	I	A2,R
	c72_mov	B1, R, A1
	c72_mov	B1, R, R

%a,n
	?2push	%cl
	FS
	S
	c72_mov	b, R, %cl
	Il	%cl, (%esp)
	pop	R
	c72_mov	B1, R, A1
	c72_mov	B1, R, R
	?2pop	%cl

%n*,c
	F*
	c72_mov	B1, #1(R), R1
	I	A2, R1
	c72_mov	B1, R1, #1(R)
	c72_mov	B1, R1, R

%n*,n
	?2push	%cl
	FS*
	S
	c72_mov	b, R, %cl
	pop %edi
	c72_mov	B1, (%edi), R
	I	%cl, R
	c72_mov	B1, R, (%edi)
	c72_mov	B1, R, R
	?2pop	%cl

/* =+, =-, =|, =& */
cr70:
%aw,cw
	Il	A2,A1
	mov	A1,R

%aw,n
	S
	I	R,A1
	mov	A1,R

%a,n
	S
	c72_mov	B1, A1, R1
	I	R, R1
	c72_mov	B1, R1, A1
	c72_mov	B1, R1, R

%ew*,cw
	F*
	Il	A2,#1(R)
	mov	#1(R),R

%ew*,n
	S
	F1*
	I	R,#1(R1)
	mov	#1(R1),R

%n*,aw
	F*
	c72_mov	B1,#1(R),R1
	I	A2,R1
	c72_mov	B1,R1,#1(R)
	c72_mov	B1,R1, R

%nw*,n
	SS
	F*
	pop	%edi
	I	%edi,#1(R)
	mov	#1(R),R

%n*,n
	SS
	F*
	pop	%edi
	c72_mov	B1,#1(R),R1
	I	%edi,R1
	c72_mov	B1,R1,#1(R)
	c72_mov	B1,R1, R

/* int -> int[] */
cr97:
%n,n
	F
	shr	$2, R

.data
.balign 4
.text

