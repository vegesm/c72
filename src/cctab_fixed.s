/* c code tables-- set condition codes */

.globl	_cctab

_cctab=.;.int .+4
	.int 20, rest
	.int 21, rest
	.int 22, rest
	.int 30, rest
	.int 31, rest
	.int 34, rest
	.int 35, rest
	.int 36, rest
	.int 37, rest
	.int 40, rest
	.int 41, rest
	.int 42, rest
	.int 43, rest
	.int 44, rest
	.int 45, rest
	.int 46, rest
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
	.int 72, rest
	.int 73, rest
	.int 74, rest
	.int 75, rest
	.int 76, rest
	.int 77, rest
	.int 78, rest
	.int 79, rest
	.int 80, rest

/* relationals */
cc60:
%a,z
	tstB1	A1

%n*,z
	F*
	tstB1	#1(R)

%n,z
	F
	tst	R

%a,a
	cmpBE	A1,A2

%n*,a
	F*
	cmpBE	#1(R),A2

%n,a
	F
	cmpB2	R,A2

%n*,e*
	F*
	S1*
	cmpBE	#1(R),#2(R1)

%n*,e
	F*
	S1
	cmpB1	#1(R),R1

%n,e*
	F
	S1*
	cmpB2	R,#2(R1)

%n,e
	F
	S1
	cmp	R,R1

%n*,n*
	FS*
	S*
	cmpBE	(sp)+,#2(R)

%n*,n
	FS*
	S
	cmpB1	*(sp)+,R

%n,n*
	FS
	S*
	cmpB2	(sp)+,#2(R)

%n,n
	FS
	S
	cmp	(sp)+,R

/* set codes right - put here those instructions that end with add/or/sub/etc (no mov). Those set the flags correctly */
rest:
%n,n
	H

.data
.balign 4
.text

