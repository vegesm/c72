/* c code tables-- set condition codes */

.globl	_cctab

_cctab=.;.int .+4
	.int 30, rest
	.int 31, rest
	.int 37, rest
	.int 40, rest
	.int 41, rest
	.int 47, rest
	.int 48, rest
	.int 60, cc60
	.int 61, cc60
	.int 62, cc60
	.int 63, cc60
	.int 64, cc60
	.int 65, cc60
	.int 66, cc60
	.int 67, cc60
	.int 68, cc60
	.int 69, cc60
	.int 70, rest
	.int 71, rest
	.int 77, rest
	.int 78, rest
	.int 79, rest

/* relationals */
cc60:
/* this prevents using %aw,cw template, because x86 allows a single immediate only */
%c,c
	F
	cmp	A2,R

%aw,cw
	cmpl	A2,A1

%aw,a
	S
	cmp	R,A1

%nw*,aw
	F*
	cmpl	A2,#1(R)

%n,aw
	F
	cmp	A2,R

%nw*,e
	F*
	S1
	cmp	R1,#1(R)

%n,ew*
	F
	S1*
	cmp	#2(R1),R

%n,e
	F
	S1
	cmp	R1,R

%n,n
	FS
	S
	pop %edi
	cmp	R,%edi

/* these opcodes set the flags correctly, no need to call an extra test */
rest:
%n,n
	H

.data
.balign 4
.text

