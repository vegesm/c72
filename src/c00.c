/* C compiler

Copyright 1972 Bell Telephone Laboratories, Inc.

*/

#include "c0.h"
#include <stdlib.h>

ossiz = 250;
int ospace[250];	/* fake */

/*
 * Adds the string s with id t to the hash table.
 */
init(s, t)
char s[]; {
	char *sp;
	int *np, i;

	/* copy s to symbuf */
	i = namsiz;
	sp = symbuf;
	while(i--)
		if ((*sp++ = *s++)=='\0') --s;
	np = lookup();
	*np++ = 1;
	*np = t;
}

/*
 * First pass of the C compiler. It parses the input and generates an intermediate(?) output,
 * containing parsed expression trees with some additional assembly code.
 *
 * Short overview: the compiler parses global elements, which can be either functions or
 * global variable declarations. In functions, each expression is parsed into a tree, which is stored
 * in ospace. Interestingly, the next pass of the compiler is expected to be loaded at the same location as this pass.
 * So, the child node pointers in the tree are simply saved directly in the output and are expected to be loaded
 * back at the same memory location.
 *
 * The tree parser also does a rough estimation of how many registers are needed for calculating a tree
 * using the Sethi-Ullman algorithm. Additionally, sides of a binary operation can be flipped such that
 * the more difficult subtree comes first. See the C compiler tour in the UNIX manual.
 */
main(argc, argv)
char *argv[]; {
	if(argc<3) {
		error("Arg count");
		exit(1);
	}
	if(freopen(argv[1], "r", stdin)==NULL) {
		error1("Can't find %s", argv[1]);
		exit(1);
	}
	if((fout=fopen(argv[2], "w"))==NULL) {
		error1("Can't create %s", argv[2]);
		exit(1);
	}
//	fout = stdout;

	init("int", 0);
	init("char", 1);
	init("float", 2);
	init("double", 3);
/*	init("long", 4);  */
	init("auto", 5);
	init("extern", 6);
	init("static", 7);
	init("goto", 10);
	init("return", 11);
	init("if", 12);
	init("while", 13);
	init("else", 14);
	init("switch", 15);
	init("case", 16);
	init("break", 17);
	init("continue", 18);
	init("do", 19);
	init("default", 20);
	while(!eof) {
		extdef();
		blkend();
	}
	fflush(stdout);

	exit(nerror!=0);
}

/*
 * Looks up an element in the hash table. The key is in symbuf.
 * Returns a pointer to the 4 word long data section.
 * Following these 4 words, another 4 words contain the key.
 */
int *lookup() {
	auto i, j, *np, *sp, *rp;

	i = 0;
	sp = symbuf;
	j = nwps;
	while(j--)
		i += *sp++;
	if (i<0) i = -i;
	i %= hshsiz;  /* the hash of symbuf */

	i *= pssiz;
	while(*(np = &hshtab[i+4])) {
		sp = symbuf;
		j = nwps;
		while(j--)
			if (*np++ != *sp++) goto no;  /* key does not match, go to next one */
		return(&hshtab[i]);
no:		if ((i += pssiz) >= hshlen) i = 0;
	}

	/* not found, add new element */
	if(hshused++ > hshsiz) {
		error("Symbol table overflow");
		exit(1);
	}
	rp = np = &hshtab[i];
	sp = symbuf;
	j = 4;
	while(j--)  /* clear out data */
		*np++ = 0;
	j = nwps;
	while(j--) /* copy key into &hshtab[i+4] */
		*np++ = *sp++;
	return(rp);
}

/*
 * The lexer, returns the opcode of the next symbol. If the caller does not want to use the symbol,
 * it can "push it back" using the peeksym global variable.
 *
 * The return value is the opcode of the current symbol. See c0t.c for the mapping between
 * the symbols and numbers.
 * If the current symbol is a number/character literal, the cval variable is set to the numeric value.
 * If the current symbol is a string, cval contains its label.
 * If the current symbol is a keyword, cval contains its id.
 * If the current symbol is a name, csym will point to the corresponding entry in the hashtable.
 * See csym comments at the bottom for its contents.
 */
symbol() {
	auto b, c;
	char *sp;

	if (peeksym>=0) {  /* if we have a peeked symbol, return that */
		c = peeksym;
		peeksym = -1;
		return(c);
	}
	if (peekc) {  /* use peeked character, if has one */
		c = peekc;
		peekc = 0;
	} else
		if (eof)
			return(0); else
			c = getchar();
loop:
	switch(ctab[c]) {

	case 125:	/* newline */
		line++;

	case 126:	/* white space */
		c = getchar();
		goto loop;

	case 0:		/* EOF */
		eof++;
		return(0);

	case 40:	/* + */
		return(subseq(c,40,30));

	case 41:	/* - */
		return(subseq(c,41,31));

	case 80:	/* = */
		if (subseq(' ',0,1)) return(80);
		c = symbol();
		if (c>=40 & c<=49)  /* c is binary operator */
			return(c+30);
		if (c==80)   /* = */
			return(60);
		peeksym = c;
		return(80);

	case 63:	/* < */
		if (subseq(c,0,1)) return(46);
		return(subseq('=',63,62));

	case 65:	/* > */
		if (subseq(c,0,1)) return(45);
		return(subseq('=',65,64));

	case 34:	/* ! */
		return(subseq('=',34,61));

	case 43:	/* / */
		if (subseq('*',1,0))
			return(43);
com:
        /* inside a comment */
		c = getchar();
com1:
		if (c=='\0') {
			eof++;
			error("Nonterminated comment");
			return(0);
		}
		if (c=='\n')
			line++;
		if (c!='*')
			goto com;
		c = getchar();
		if (c!='/')
			goto com1;
		c = getchar();
		goto loop;

	case 124:	/* number */
		cval = 0;
		if (c=='0')
			b = 8; else
			b = 10;
		while(ctab[c]==124) {
			cval = cval*b + c -'0';
			c = getchar();
		}
		peekc = c;
		return(21);

	case 122:	/* " */
		return(getstr());

	case 121:	/* ' */
		return(getcc());

	case 123:	/* letter */
		sp = symbuf;
		while(ctab[c]==123 | ctab[c]==124) {  /* while c is alphanumeric */
			if (sp<((char *)symbuf)+namsiz) *sp++ = c;
			c = getchar();
		}
		while(sp<((char *)symbuf)+namsiz)
			*sp++ = '\0';
		peekc = c;
		csym = lookup();  /* find in hashtable */
		if (csym[0]==1) {	/* keyword */
			cval = csym[1];
			return(19);
		}
		return(20);

	case 127:	/* unknown */
		error("Unknown character");
		c = getchar();
		goto loop;

	}
	return(ctab[c]);
}

/*
 * Peeks at the next char and if it is c then eats it and returns b, otherwise returns a.
 * Useful for two character symbols, e.g. distinguish between ! and =
 * call subseq('=', note_equal_code, logical_not_code).
 */
subseq(c,a,b) {
	if (!peekc)
		peekc = getchar();
	if (peekc != c)
		return(a);
	peekc = 0;
	return(b);
}

/*
 * Gets a string. It assumes the opening quotation mark has been already processed.
 */
getstr() {
	auto c;

	printf(".data;l%d:.byte ", cval=isn++);
	while((c=mapch('"')) >= 0)
		printf("%d,", c);
	printf("0;.balign 4;.text\n");
	return(22);
}

/*
 * Reads a character literal. Assumes opening ' has been read already.
 */
getcc()
{
	auto c, cc;
	char *cp;

	cval = 0;
	cp = &cval;
	cc = 0;
	while((c=mapch('\'')) >= 0)
		if(cc++ < ncpw)
			*cp++ = c;
	if(cc>ncpw)
		error("Long character constant");
	return(21);
}


/*
 * Processes a character from a string/character literal. c contains the delimiter char.
 * This function handles mapping of escape sequences.
 */
mapch(c)
{
	auto a;

	if((a=getchar())==c)
		return(-1);
	switch(a) {

	case '\n':
	case 0:
		error("Nonterminated string");
		peekc = a;
		return(-1);

	case '\\':
		switch (a=getchar()) {

		case 't':
			return('\t');

		case 'n':
			return('\n');

		case '0':
			return('\0');

		case 'r':
			return('\r');

		case '\n':
			line++;
			return('\n');
		}

	}
	return(a);
}

/*
 * Builds an expression tree. The outline of the algorithm:
 * There are three stacks:
 * cmst - tree node stack, contains the partially built parts of the expression tree.
 * opst - operator stack, contains the ids of the operators. The bottom element is the EOF operator
 * prst - precedence stack, contains the precedences of operators. Note that the precedence stack only contains
 * element, if the precedence has increased, so it is not in one-to-one correspondence with opst.
 *
 * The algorithm goes over the operator produced by the symbol function. Leaf nodes (numbers,
 * string/char literals, names) are placed on the node stack. If an operator is encountered,
 * it is placed on the operator stack if its precedence is higher than the current precedence.
 * Otherwise, the operator and tree node stack is unwinded by building partial subtrees,
 * until the top of the prst is smaller than current operator's precedence.
 */
tree() {
	auto *op, opst[20], *pp, prst[20], andflg, o,
		p, ps, os;

	space = ospace;
	op = opst;  /* top of operator stack */
	pp = prst;  /* top of precedence stack */
	cp = cmst;  /* top of tree node stack */
	*op = 200;		/* stack EOF */
	*pp = 06;
	andflg = 0;

advanc:
	switch (o=symbol()) {

	/* name */
	case 20:
		if (*csym==0)  /* storage not yet decided */
			if((peeksym=symbol())==6)  /* (, mark function calls as extern */
				*csym = 6;	/* extern */
			else {
				if(csym[2]==0)	/* unseen so far */
					csym[2] = isn++;
			}
        if(*csym==6)	/* extern */
            *cp++ = block(5,20,csym[1],0,*csym,
                csym[4],csym[5],csym[6],csym[7]);
        else
            *cp++ = block(2,20,csym[1],0,*csym,csym[2]);
		goto tand;

	/* short constant */
	case 21:
	case21:
		*cp++ = block(1,21,ctyp,0,cval);  /* ctyp is always 0 */
		goto tand;

	/* string constant */
	case 22:
		*cp++ = block(1,22,17,0,cval);  /* 17 is char[] */

tand:
		if(cp>=cmst+cmsiz) {
			error("Expression overflow");
			exit(1);
		}
		if (andflg)
			goto syntax;
		andflg = 1;
		goto advanc;

	/* ++, -- */
	case 30:
	case 31:
		if (andflg)  /* convert to postfix */
			o += 2;
		goto oponst;

	/* ! */
	case 34:
		if (andflg)
			goto syntax;
		goto oponst;

	/* - */
	case 41:
		if (!andflg) {
			peeksym = symbol();
			if (peeksym==21) {  /* negative literal */
				peeksym = -1;
				cval = -cval;
				goto case21;
			}
			o = 37;
		}
		andflg = 0;
		goto oponst;

	/* & */
	/* * */
	case 47:
	case 42:
		if (andflg)
			andflg = 0; else
			if(o==47)
				o = 35;
			else
				o = 36;
		goto oponst;

	/* ( */
	case 6:
		if (andflg) {
		    /* this is a function call */
			o = symbol();
			if (o==7)  /* ) */
				o = 101; else {  /* 101 - call without args */
				peeksym = o;
				o = 100;
				andflg = 0;
			}
		}
	goto oponst;

	/* ) */
	/* ] */
	case 5:
	case 7:
		if (!andflg)
			goto syntax;
		goto oponst;
	}

	/* binary operators */
	if (!andflg)
		goto syntax;
	andflg = 0;

oponst:  /* place operator on stack */
	p = (opdope[o]>>9) & 077;  /* extract operator precedence */
opon1:
	ps = *pp;  /* currently highest precedence */
	if (p>ps | p==ps & (opdope[o]&0200)!=0) { /* right-assoc */
putin:
		switch (o) {

		case 6: /* ( */
		case 4: /* [ */
		case 100: /* call */
			p = 04;
		}
		if(op>=opst+20) {		/* 20 is the size of opstack */
			error("expression overflow");
			exit(1);
		}
		*++op = o;
		*++pp = p;
		goto advanc;
	}
	--pp;  /* pop precedence stack */
	switch (os = *op--) {

	/* EOF */
	case 200:
		peeksym = o;
		return(*--cp);

	/* call */
	case 100:
		if (o!=7)  /* unmatching parenthesis */
			goto syntax;
		build(os);
		goto advanc;

	/* mcall */
	case 101:
		*cp++ = 0;		/* 0 arg call */
		os = 100;
		goto fbuild;

	/* ( */
	case 6:
		if (o!=7)  /* unmatching parenthesis */
			goto syntax;
		goto advanc;

	/* [ */
	case 4:
		if (o!=5)
			goto syntax;
		build(4);
		goto advanc;
	}
fbuild:
	build(os);
	goto opon1;  /* unwinds precedence stack till at the same level as o */

syntax:
	error("Expression syntax");
	errflush(o);
	return(0);
}

/*
 * Processes a variable declaration, the preceding type/storage keyword has been processed already.
 * kw - the id of the preceding type/storage keyword, 8 for function parameter list.
 */
void declare(kw) {
	int o;

	while((o=symbol())==20) {		/* name */
		if(kw>=5) {			/* type or storage location keyword? */
			if(*csym>0)
				error1("%p redeclared", &csym[4]);  /* storage area redeclared */
			*csym = kw;
		} else {
			if ((csym[1]&017)!=0)  /* type already defined, adding pointer behavior is allowed */
				error1("%p redeclared", &csym[4]);
			csym[1] |= csym[1]&0760 | kw;  // set kw to lower 4 bits
			if (*csym==0)
				*csym = -2;
		}
		/* add pointer indirection */
		while((o=symbol())==4) {	/* [ */
			if((o=symbol())==21) {	/* const */
				if(csym[1]>=020)
					error("Bad vector");
				csym[3] = cval;
				o = symbol();
			}
			if (o!=5)		/* ] */
				goto syntax;
			csym[1] += 020;
		}
		if(kw==8)  {		/* parameter */
			*csym = -1;
			if (paraml==0) /* paraml points to the first element in the parameter list */
				paraml = csym;
			else
				*parame = csym;  /* set previous parameter's first word to point to the current parameter */
			parame = csym;
		}
		if (o!=9)	/* , */
			break;
	}
	if(o==1 & kw!=8 | o==7 & kw==8)  /* not parameter list and ; or parameter list and ) */
		return;
syntax:
	error("Declaration syntax");
	errflush(o);
}

/* constants for code generator tables */
regtab = 0;
efftab = 1;
cctab = 2;
sptab = 3;

/* hash table */
symbuf[2];  /* buffer for the key to look up in has table. 8 byte long, should be 8/sizeof(int); original value was 4 */
pssiz = 8;  /* size of an entry in the hashtable in words,  should be 4 + nwps */
namsiz = 8;  /* maximum length of the key in bytes */
nwps = 2;  /* number of words per symbuf - originally 4 */
hshused = 0;  /* number of elements in the hash table */
hshsiz= 100;  /* maximum number of elements in the table */
hshlen =800;	/* size of the table in word, equals to pssiz*hshsiz */
hshtab[800];  /* The hash table for symbols. For eahc entry, the first 4 bytes are the data, the next 4 bytes are the key. */

int *space= 0;
int *cp= 0;  /* top of the cmst stack */
cmsiz= 40;  /* size of the cmst stack */
cmst[40];  /* the tree node stack, contains pointers to the nodes*/
ctyp = 0; /* id of the int type, constant */
isn = 1;  /* current label number */
swsiz = 120;
swtab[120];
int *swp = 0;
contlab = 0;  /* label for a continue statement in the current loop */
brklab = 0;   /* label for a break statement in the current loop */
deflab = 0;   /* label for a deafult statement in the current switch */
nreg = 4;  /* number of general registers available */
maprel[]={ 60,61,64,65,62,63,68,69,66,67};  /* maps binary relations to the their flipped pairs */
nauto = 0;
stack = 0;
peeksym = -1;  /* peeked symbol */
peekc= 0;  /* peeked character */
eof = 0;  /* true if reached end of file */
line = 1;  /* current line */
int *csym = 0;  /* current symbol see meaning below */
cval = 0;  // contains the currently read character literal
ncpw = 4;  /* number of characters per word */
nerror = 0;  /* number of errors during parsing */
FILE *fout;  /* putchar prints characters to this file */
int *paraml;  /* head of the parameter list */
int *parame;  /* last element in the parameter list */


/*
 * csym - current symbol description, points to the corresponding element in the hash table.
 * Meaning of the 4 words
 * 0 - if 1 then keyword, otherwise storage scope (keyword ID), 8 for function parameters
 *        -1 (temporary) for function parameters csym[0] forms a linked list, -1 marks the end
 *        -2 default storage scope (auto)
 *        1 - keyword
 *        2 - label
 *        5 - auto
 *        6 - extern
 *        7 - static
 *       10 - function parameter (set by blkhed)
 * 1 - type description
 *        lower 4 bits contain raw type (char/double/int)
 *        then 020 is added for every indirection
 *        e.g. 020 refers to int[]
 * 2 - location/label
 *     in case of parameters/auto the offset from the stack frame
 *     in case of statics, the label of the variable
 * 3 - the length in case of arrays, 0 otherwise
 * 4 - the name of the symbol
 *
 * cval - value in the current symbol
 * if number literal - the number
 * if char literal - character ascii code
 * if keyword - the keyword ID
 * if string - the label in the assembly output
 */
