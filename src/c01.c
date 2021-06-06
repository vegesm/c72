#include "c0.h"
#include <stdlib.h>
#include <stdarg.h>

/*
 * Builds a new node of the operator tree and pushes it onto the cmst stack.
 * The necessary parameters are popped from cmst.
 */
void build(int op) {
	int *p1, t1, d1, *p2, t2, d2, t;
	int d, dope, lr, cvn;

    /* replace a[b] with *(a+b) */
	if (op==4)  {		/* [] */
		build(40);  /* + */
		op = 36;
	}
	dope = opdope[op];
	if ((dope&01)!=0) {  /* binary */
		p2 = *--cp; /* second subtree */
		if(p2 != 0) {  /* if op was mcall originally, there is a fake 0 null pointer here, ignore it */
            t2 = p2[1];
            d2 = p2[2];
        }
	}
	p1 = *--cp;  /* first subtree */
	t1 = p1[1];
	d1 = p1[2];
	switch (op) {

	/* , */
	case 9:
		*cp++ = block(2, 9, 0, 0, p1, p2);
		return;

	/* ? */
	case 90:
		if (*p2!=8)
			error("Illegal conditional");
		goto goon;

	/* call */
	case 100:
		*cp++ = block(2,100,t1,24,p1,p2);
		return;

	/* * */
	case 36:
		if ((t1 -= 16)<0)  {
			error("Illegal indirection");
			t1 += 16;
		}
		if (*p1!=20 & d1==0)
			d1 = 1;
		*cp++ = block(1,36,t1,d1,p1);
		return;

	/* & unary */
	case 35:
		if (*p1 == 36) {	/* * - shorten &*p to p */
			*cp++ = p1[3];
			return;
		}
		if (*p1 == 20) {  /* name */
			*cp++ = block(1,p1[3]==5?29:35,t1+16,1,p1); /* is it auto? */
			return;
		}
		error("Illegal lvalue");
	}
goon:
	if ((dope&02)!=0)		/* lvalue needed on left? */
		chklval(p1);
	if ((dope&020)!=0)		/* word operand on left? */
		chkw(p1);
	if ((dope&040)!=0)		/* word operand on right? */
		chkw(p2);
	if ((dope&01)!=0) {		/* binary op? */
		cvn = cvtab[9*lintyp(t1)+lintyp(t2)];  /* conversion from t1 to t2 ?? */
 		if ((dope&010)!=0)  {	/* assignment? */
			t = t1;
			lr = 1;  /* right argument must be converted */
			cvn &= 07;
		} else {
			t = (cvn&0100)!=0? t2:t1;  /* who determines the result type */
			lr = cvn&0200;
			cvn = (cvn>>3)&07;
		}
		if (cvn) {
			if (cvn==07) {
				error("Illegal conversion");
				goto nocv;
			}
			cvn += (dope&010)!=0? 83:93;  /* assignment? */
			if (lr) {  /*  right argument needs to be converted */
				t2 = t;
				 d2 = (p2=convert(p2, t, d2, cvn))[2];  /* this also emits the conversion operation */
			} else {
				t1 = t;
				d1 = (p1=convert(p1, t, d1, cvn))[2];
			}
nocv:;		}
		if (d2>d1 && (dope&0100)!=0) {	/* flip operands, if second tree is more difficult */
			if ((dope&04)!=0)	/* relational? */
				op = maprel[op-60];
			d = d1;
			d1 = d2;
			d2 = d;
			d = p1;
			p1 = p2;
			p2 = d;
			d = t1;
			t1 = t2;
			t2 = d;
		}
		if (d1==d2)  /* calculating registers needed for this node using Sethi-Ullman */
			d = d1+1; else
			d = max(d1,d2);
		if ((dope&04)!=0)
			t = 0;		/* relational ops have integer type */
		*cp++ = block(2,op,t,d,p1,p2);
		return;
	}
	*cp++ = block(1,op,t1,d1==0?1:d1,p1);  /* unary operator */
}

/*
 * Creates a node for type conversion. This operation can be in place,
 * i.e. modifies an existing node in the tree.
 * t - new type
 * d - current difficulty
 * cvn - conversion opcode
 */
int *convert(int *p, int t, int d,  int cvn) {
	int c;
	if (*p==21) {		/* constant */
		c = p[3];  /* the current value of the constant */
		switch(cvn) {

		case 99:		/* c18, int -> double[] */
			c <<= 1;

		case 98:		/* c14, int -> float[] */
		case 97:		/* c12, int -> int[] */
			c <<= 2;

			p[3] = c;
		return(p);
		}
	}
	return(block(1, cvn, t, max(1,d), p));
}

/* check if p is a word type */
void chkw(int *p)  {
	int t;

	if ((t=p[1])>1 && t<16)
		error("Integer operand required");
}

/* Index of type in cvtable */
int lintyp(int t) {
	return(t<16? t:(t<32? t-12: 8));
}


void error(char *s) {
    error2(s, 0, 0);
}

void error1(char *s, int p1) {
    error2(s, p1, 0);
}

void error2(char *s, int p1, int p2) {
	FILE *f;

	nerror++;
	fflush(fout);
	f = fout;
	fout = stderr;
	printf("%d: ", line);
	printf(s, p1, p2);
	putchar('\n');
	fflush(stderr);
	fout = f;
}

/*
 * Creates a new node and appends it to the end of *ospace. The parameters of the node
 * are passed after the argument n, which contains the number of optional parameters.
 * Each node has 3 mandatory parameters and any number of optional ones. The mandatory parameters:
 * op - operator id
 * t - type
 * d - difficulty, number of registers to calculate the node, or statement difficulty level in c1
 * These are followed by the (op dependent) optional parameters, which usually are the subtrees.
 */
int *block(int n, ...)
{
	auto *p;
	va_list arguments;

#ifdef DEBUG
	va_list dbg_args;
	va_start(dbg_args, n);
	int op = va_arg(dbg_args, int);
	int type = va_arg(dbg_args, int);
	int regcnt = va_arg(dbg_args, int);
	printf("loc %d: %d t=%d d=%d nump=%d ", space-ospace, op, type, regcnt, n);
    for (int i = 0; i < n; ++i) {
        int d = va_arg(dbg_args, int);
        printf("%d (%d) ", d, (int*)d-ospace);
    }
    printf("\n");
    va_end(dbg_args);
#endif

	p = space;
	va_start(arguments, n);
	n += 3;
	if(space+n >= ospace+ossiz) {
		error("Expression overflow");
		exit(1);
	}
	while(n--)
		*space++ = va_arg(arguments, int);

	va_end(arguments);
	return(p);
}

/* check if p is an lvalue (name or pointer) */
void chklval(p)
int p[]; {
	if (*p!=20)
		if (*p!=36)
			error("Lvalue required");
}

int max(a, b)
{
	if (a>b)
		return(a);
	return(b);
}

