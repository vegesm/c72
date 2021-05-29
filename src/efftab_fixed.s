/* c code tables */

.globl	_efftab

_efftab=.;.int .+4
	.int 30, ci30
	.int 31, ci30
	.int 32, ci30	/* same as 30 */
	.int 33, ci30	/* same as 31 */
	.int 80, ci80
	.int 70, cr70
	.int 71, cr70
	.int 77, cr70
	.int 78, cr70
	.int 0

/* ++ prefix */
ci30:
%ai,n
%abp,n
%ab,n
	IB1	A1

%aip,n
	I'l	$4,A1

%nbp*,n
%ni*,n
%nb*,n
	F*
	IB1	#1(R)

%nip*,n
	F*
	I'l	$4,#1(R)

/* = */
ci80:
%a,c
	c72_mov	B1, A2,A1

%a,n
	S
	c72_mov	B1, R,A1

%n*,c
	F*
	c72_mov	B1, A2,#1(R)

%n*,e
	F*
	S1
	c72_mov	B1, R1,#1(R)

%n*,n
	FS*
	S
	pop %edi
	c72_mov	B1, R,(%edi)

/* =+, =-, =|, =& */
cr70:
%aw,cw
	Il	A2,A1

%aw,n
	S
	I	R,A1

%a,n
	S
	c72_mov	B1, A1, R1
	I	R, R1
	c72_mov	B1, R1, A1

%ew*,cw
	F*
	Il	A2,#1(R)

%ew*,n
	S
	F1*
	I	R,#1(R1)

%n*,aw
	F*
	c72_mov	B1,#1(R),R1
	I	A2,R1
	c72_mov	B1,R1,#1(R)

%nw*,n
	SS
	F*
	pop	%edi
	I	%edi,#1(R)

%n*,n
	SS
	F*
	pop	%edi
	c72_mov	B1,#1(R),R1
	I	%edi,R1
	c72_mov	B1,R1,#1(R)

.data
.balign 4

