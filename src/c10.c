/*

	    	C compiler, part 2

	Copyright 1972 Bell Telephone Laboratories, Inc.

*/
#include "c1.h"
#include <stdlib.h>

ospace[1000];	/* fake */

/*
 * Second pass of the compiler. The code was expected to be loaded at the same location as the first pass,
 * such that ospace is at the same location as in the first pass. Because of this, pointers in the
 * expression trees were saved using  the absolute address. The fixp function converts the incorrect addresses
 * to ones pointing into this pass' ospace. Throughout this code, unfixed pointer means the fixp function
 * hasn't been called for the pointer yet.
 *
 * For details of the algorithm, see the "A Tour through the UNIX C Compiler" in the UNIX v7 manual.
 * Though it is for a newer version of C, almost everything is applicable to this compiler.
 * The difficulty levels used in this compiler:
 * z -  4 - zero
 * c -  8 - number
 * i - 12 - name/string/float/double, pointer address(&)
 * a - 16 - addressible by PDP11 address instructions
 * e - 20 - easy statement, value can be calculated using the available registers only
 * n - 63 - anything
 */
void main(argc, argv)
char *argv[];
{
	int *sp, c, *table, *tabtab[4], tree;

	if (argc<2) {
		error("Arg count");
		exit(1);
	}
	if(freopen(argv[1], "r", stdin)==NULL) {
		error1("Can't find %s", argv[1]);
		exit(1);
	}
	fout = stdout;
    printf(".include \"C:/projectek/legacy-cc72/src/pdp11_mov.s\"\n");

	tabtab[0] = regtab;
	tabtab[1] = efftab;
	tabtab[2] = cctab;
	tabtab[3] = sptab;
	while((c=getchar())>0) {
		if(c=='#') {  /* expression */
			sp = ospace;
			baseptr=getwrd();
			c = getwrd();
			tree = getwrd();
			table = tabtab[getwrd()];
			line = getwrd();
			while(c--)
				*sp++ = getwrd();
			rcexpr(tree, table, 0);
		} else
			putchar(c);
	}

	exit(nerror!=0);
}

/*
 * Finds a code generation template in the generation table.
 * Returns a pointer to the template string.
 *
 * tree is an unfixed pointer
 * nreg - number of available registers
 */
char *match(tree, table, nreg)
int tree[], table[]; {
	int op, d1, d2, t1, t2, *p1, *p2;
	char *mp;

	/* t1/t2 - type of the result of subtree, d1/d2 - difficulty of the subtree */

	if (tree==0)
		return(0);

	tree = fixp(tree);
	op = *tree;
	if (op>=29)			/* if not leaf */
		p1 = fixp(tree[3]);
	else
		p1 = tree;
	t1 = p1[1];
	d1 = dcalc(p1, nreg);
	if ((opdope[op]&01)!=0) {	/* binary? */
		if(tree[4]!=0) { /* in function calls second argument is empty at this point */
    		p2 = fixp(tree[4]);
            t2 = p2[1];
            d2 = dcalc(p2, nreg);
        } else {
		    p2 = 0;
		}
	}

	/* Look up the entries for this operator */
	while(*table) {
		if (*table++ == op) goto foundop;
		table++;
	}
	return(0);
foundop:
	table = *table;
nxtry:
    /* Iterate over the list of templates for this op */
	mp = table;
	if (*mp == 0)
		return(0);
	if (d1 > (*mp&077) | (*mp>=0100)&(*p1!=36)) /* tree1 has a difficulty level <= than this condition, or this is a pointer */
		goto notyet;
	if (notcompat(t1, mp[1]))
		goto notyet;
	if ((opdope[op]&01)!=0 & p2!=0) {  /* binary op, check second argument */
		if (d2 > (mp[2]&077) | (mp[2]>=0100)&(*p2!=36))
			goto notyet;
		if (notcompat(t2,mp[3]))
			goto notyet;
	}
now:
	return(table[1]);  /* parameters are 4bytes = 1 word */
notyet:
	table = table+2;  /* jump to next entry in table (on entry is 4 bytes + 1 sizeof(int *) ) */
	goto nxtry;
}

/*
 * Generates code for this expression. If the instruction does not have a matching entry
 * in one of the specialized code tables, it falls back to regtab and moves the result
 * to the appropriate place.
 *
 * tree is an unfixed pointer
 * reg - result should go into this register if table==regtab;
 *       Additionally, the code generator should leave lower numbered registers alone,
 *       and only use reg or higher numbered registers.
 */
void rcexpr(tree, table, reg)
int tree[], table[]; {
    int *origtree;

	if(tree==0)
		return;

	origtree = tree;
	tree = fixp(tree);

	if(*tree >= 103) {  /* conditional jump */
	    cbranch(tree[1],tree[2],tree[3],0);
		return;
	}
	if(table == cctab || table==regtab) {  // temporarily only these two tables are enabled
	    if (cexpr(origtree, table, reg))
		    return;
	}

	if (table!=regtab) {
        if (cexpr(origtree, regtab, reg)) {
            if (table == sptab) {
                printf("push\t");
                printreg(reg);
                putchar('\n');
            }
            if (table == cctab) {
                printf("test	");
                printreg(reg);
                putchar(',');
                printreg(reg);
                putchar('\n');
            }
            return;
        }
    }
	error1("No match for op %d", *tree);
}

/*
 * Generate code using the given table. If a template was found
 * and the code successfully generated then returns 1, otherwise 0.
 *
 * tree is an unfixed pointer
 * reg - result should go into this register if table==regtab;
 *       Additionally, the code generator should leave lower numbered registers alone,
 *       and only use reg or higher numbered registers.
 */
int cexpr(tree, table, reg)
int tree[], table[]; {
	int *p1, *fp1, *p2, *fp2, c, r, *p, *otable, *ctable, *origtree;
	char *string;

	origtree = tree;
	tree = fixp(tree);

	if ((c = *tree)==100) {		/* function call, push params on stack */
		p1 = tree[3];
		p2 = tree[4];
		fp2 = fixp(p2);
		r = 0;
		if(p2) {
			while (*fp2==9) { /* comma */
				rcexpr(fp2[4], sptab, 0);
				r += rlength(fixp(p=fp2[4])[1]);
				p2 = fp2[3];
				fp2 = fixp(p2);
			}
			rcexpr(p2, sptab, 0);
			r += rlength(fp2[1]);
		}
		*tree = 101;
		tree[2] = r;		/* save arg length */
	}
	if(c==90) {		/* ? */
		cbranch(tree[3], c=isn++, 0, reg);
		rcexpr(fixp(tree[4])[3], table, reg);
		branch(r=isn++, 0, 0);
		label(c);
		rcexpr(fixp(tree[4])[4], table, reg);
		label(r);
		return(1);
	}
	if ((string=match(origtree, table, nreg-reg))==0)  /* find code template */
		return(0);
	p1 = tree[3];
	p2 = tree[4];
	fp1 = fixp(p1);
	fp2 = fixp(p2);
loop:
	switch(c = *string++) {

	case '\0':
		p = tree;
		if (*p==101 & p[2]>0) {  /* if this was a function call, pop parameters from stack. */
			popstk(p[2]);
		}
		return(1);

	/* A1 */
	case 'A':
		p = fixp(tree[3]);
		goto adr;

	/* A2 */
	case 'B':
		p = fixp(tree[4]);
		goto adr;

	/* A */
	case 'O':
		p = tree;
	adr:
		pname(p);
		goto loop;

	/* I */
	case 'M':
		if ((c = *string)=='\'')
			string++; else
			c = 0;
		prins(*tree, c);
		goto loop;

	/* B1 */
	case 'C':
		p = fixp(tree[3]);
		goto pbyte;

	/* BF */
	case 'P':
		p = tree;
		goto pb1;

	/* B2 */
	case 'D':
		p = fixp(tree[4]);
	pbyte:
		if (p[1]==1)	/* char type? */
			putchar('b');
pb1:
		if (isfloat(p, 0))
			putchar('f');
		goto loop;

	/* BE */
	case 'L':
		if (fixp(tree[3])[1]==1 | fixp(tree[4])[1]==1)
			putchar('b');
		goto loop;

	/* C1 */
	case 'E':
		p = fp1[3];
		goto constl;

	/* C2 */
	case 'F':
		p = fp2[3];
	constl:
		printf("%o", p);
		goto loop;

	/* F */
	case 'G':
		p = p1;
		goto subtre;

	/* S */
	case 'K':
		p = p2;
		goto subtre;

	/* H */
	case 'H':
		p = origtree;

	subtre:
		ctable = regtab;
		r = reg;
		c = *string++ - 'A';
		if ((c&02)!=0)  /* should it go to stack? */
			ctable = sptab;
		if ((c&04)!=0)  /* is it a conditional expression? */
			ctable = cctab;
		if((c&010)!=0)  /* F1, S1, H1 - using the next register */
			r = reg+1;
		if((c&01)!=0)
			if(*fixp(p)==36) { /* if this has the shape *(x+c), print tree for x only */
				p = fixp(p)[3];
				if(collcon(fixp(p)) & (ctable!=sptab))
					p = fixp(p)[3];
			}
		rcexpr(p, ctable, r);
		goto loop;

	/* R */
	case 'I':
		r = reg;
		goto preg;

	/* R1 */
	case 'J':
		r = reg+1;
	preg:
		printreg(r);
		goto loop;

	/* RL */
	case '#':
		p = fp1[3];
		goto nmbr;

	case '"':
		p = fp2[3];
		goto nmbr;
	case '~':
		p = tree[3];

	nmbr:
	    p = fixp(p);
		if(collcon(p)) {  /* If this has the form *(x+const), print const */
			c = *p;
			if(r = fixp(p=p[4])[3])
				printf("%d", c==40?r:-r);  /* + or - ? */
		}
		goto loop;

	case '?':
	    /* if current register<=r, eat this line */
	    r=*string++;
	    r-='0';
	    if(reg <= r) {
	        while(*string++ != '\n');
	    }
	    goto loop;
	/* M */
	case 'N':
		if ((c=isfloat(tree, &string))==fltmod)
			goto loop;
		printf((fltmod=c)==2?"setf\n":"setd\n");
		goto loop;

	/* Z */
	case 'Z':
		printf("%d", fp1[4]);
		goto loop;

	}
	putchar(c);
	fflush(stdout);
	goto loop;
}

/* Prints the value/label/location of a tree node. p is a fixed pointer */
void pname(p)
int *p; {
	char *np;
	int i;

loop:
	switch(*p) {

	case 21:		/* const */
		printf("$%d", p[3]);
		return;

	case 22:		/* string */
		printf("$l%d", p[3]);
		return;

	case 20:		/* name */
		switch(p[3]) {

		case 5:		/* auto, param */
			printf("%d(%ebp)", p[4]);
			return;

		/* extern */
		case 6:
			printf("%p", &p[4]);
			return;

		}
		printf("l%d", p[4]);  /* static */
		return;

	case 35:		/* & */  // TODO this should be removed probably
		putchar('$');
		p = fixp(p[3]);
		goto loop;

//	case 36:		/* * */
//		putchar('*');
//		p = fixp(p[3]);
//		goto loop;
	}
	error("pname called illegally");
}

/*
 * Difficulty level calculation.
 * p points to a fixed optable entry.
 * nreg - number of free registers.
 */
int dcalc(p, nreg)
int p[]; {
	int op, t;

	if (p==0)
		return(0);
	op = *p;
	switch (op) {

	case 20:		/* name */
	case 22:		/* string */
	case 23:		/* float */
	case 24:		/* double */
		return(12);

	case 21:		/* short constant */
		return(p[3]==0? 4:8);

	case 35:		/* & */
		return(12);

//	case 36:		/* * */
//		if ((op=dcalc(fixp(p[3]), nreg))<16)  /* if argument of pointer is constant/simple name, this is PDP-11 addressible */
//			return(16);
	}

def:
	return(p[2]<=nreg? 20: 24);  /* can this tree be calculated using nreg registers? */
}

/*
 * Checks if the two type: at and st are not compatible
 * at - type in the tree
 * st - type in the code template table
 * */
int notcompat(at, st) {

	if (st==0)		/* word, byte */
		return(at>1 & at<16);  /* can store char or int only */
	if (st==1)		/* word */
		return(at>0 & at<16);  /* can store int only */
	st -= 2;
	if (st==2 & at==3)
		at = 2;
	return(st != at);
}

/*
 * Prints the instruction belonging to this opcode. c decides whether
 * the first or the second subinstruction should be printed.
 */
void prins(op, c) {
	int *insp;
	char **insstrp;
	char *s;

	insp = instabcode;
	insstrp = instabstr;
	while(*insp) {
		if (*insp == op) {
			if ((s = insstrp[c!=0])==NULL)
				goto err;
			printf("%s", s);
			return;
		} else {
            insp = insp + 1;
            insstrp +=2;
        }
	}
err:
	error1("No match for op %d", op);
}

/* Prints register n */
void printreg(n) {
    if(n>=4) {
        error1("Too large register id: %d", n);
    }

    printf("%e");
    putchar('a' + n);
    putchar('x');
}


/* True if p is in 'x+const' form */
int collcon(p)
int p[]; {
	int *p1;

	if(*p==40 | *p==41)
		if(*(p1=fixp(p[4]))==21)
			return(1);
	return(0);
}

/* tree - correct pointer into ospace */
isfloat(t, s)
int t[];
char *s[];
{
	int rt;

	if(s!= 0) {
        rt = **s - '0';
        if (rt==2 | rt==4) {
            (*s)++;
            return(rt>2?3:2);
        }
    }
	if ((opdope[t[0]]&010)!=0)	/* relational */
		t = fixp(t[3]);
	if ((rt=t[1])>=2 && rt<=3)  /* type is float or double */
		return(rt);
	return(0);
}

int *baseptr=0;
nreg = 3;  /* number of available registers */
isn = 10000;
namsiz = 8;  /* max length of a name */
FILE* fout;
line;
tmpfil;
nerror;
fltmod;

