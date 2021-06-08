/* c code tables-- expression to -(sp) */

.globl	_sptab

_sptab=.;.int .+4
	.int 20, cs20
	.int 21, cs21
	.int 22, cs21
	.int 30, cs30
	.int 31, cs30
	.int 32, cs32
	.int 33, cs32
	.int 35, cs35
	.int 36, cs36
	.int 40, cs40
	.int 41, cs40
	.int 47, cs40
	.int 48, cs40
	.int 0


/* name */
cs20:
%aw,n
	push	A

/* constant */
cs21:
%aw,n
	push	A

/* ++,-- prefix */
cs30:
%ai,n
%abp,n
	Il	A1
	push	A1

%aw,n
	I'l	$4,A1
	push	A1

%nbp*,n
%ni*,n
	F*
	Il	#1(R)
	push	#1(R)

%nip*,n
	F*
	I'l	$4,#1(R)
	push	#1(R)
	/* TODO check this, originally push was first then inc */

/* ++,-- postfix;
only handles int and pointers, byte falls back to regtab
*/
cs32:
%ai,n
%abp,n
	push	A1
	Il	A1

%aip,n
	push	A1
	I'l	$4,A1

%nbp*,n
%ni*,n
	F*
	push	#1(R)
	Il	#1(R)

%nip*,n
	F*
	push	#1(R)
	I'l	$4,#1(R)

/* & unary of extern */
cs35:
%i,n
	push	$A1

/* * unary */
cs36:
%nbp,n
	H*
	movsbl	~(R),R
	push R

%n,n
	H*
	push	~(R)

/* + */
cs40:
%n,cw
	FS
	Il	A2,(%esp)

%n,n
	FS
	S
	Il	R,(%esp)


.data
.balign 4
.text
