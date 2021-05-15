#include "c0.h"
#include <stdarg.h>
/*
 * Records a conditional jump. If the jump is short (easystmt is true),
 * it generates a branch isntruction, otherwise a jump instruction.
 *
 * tree - tree containing a conditional expression
 * lbl - where to jump
 * cond - decides whether to jump when condition is true or false
 */
jumpc(tree, lbl, cond)
int tree[];
{
	rcexpr(block(1,easystmt()+103,tree,lbl,cond),cctab);
}

/*
 * Prints the binary representation of the tree.
 * table - the expression translation table used in the next pass.
 */
rcexpr(tree, table)
int tree[], table;  // table: the code generation table
{
	int c, *sp;

	putchar('#');
	c = space-ospace;
	sp = ospace;

	putwrd(ospace);  /* save start of array location (original code expects it to be loaded at the same place */
	putwrd(c);
	putwrd(tree);
	putwrd(table);
	putwrd(line);
	while(c--)
		putwrd(*sp++);
#ifdef DEBUG
	printf("\n");
#endif
}

jump(lab) {
	printf("jmp\tl%d\n", lab);
}

label(l) {
	printf("l%d:", l);
}

/* Generates code for a return statement. */
retseq() {
    printf("leave\nret\n");
}

/* Label for a static variable */
slabel() {
	printf(".data; l%d: 1f; .text; 1:\n", csym[2]);
}

/*
 * Reserves space on the stack.
 * a - amount of bytes to add to the stack
 */
void setstk(a) {
	auto ts;

	ts = a-stack;  /* relative distance */
	stack = a;
	switch(ts) {

	case 0:
		return;

//	case -2:	/* -2 */
//		printf("tst	-(sp)\n");
//		return;
//
//	case -4:	/* -4 */
//		printf("cmp	-(sp),-(sp)\n");
//		return;
	}
	printf("add	$%d,%esp\n", ts);
}

/* define array on stack, simply saves the pointer to the top of the stack */
int defvec() {
//	printf("mov\tsp,r0\nmov\tr0,-(sp)\n");
	printf("push\t%esp\n");
	stack -= 4;
}

/*
 * Define static variable.
 * s - pointer to symbol table entry
 */
defstat(s)
int s[]; {
	int len;

	len = length(s[1]);
	if (s[3])  /* array */
		printf(".data; l%d:1f; .bss; 1:.=.+%d; .balign 4; .text\n", s[2],
			s[3]*len);
	else  /* scalar */
		printf(".bss\n l%d:.=.+%d; .balign 4; .text\n", s[2], len);
}

/*
 * Length of the datatype, t is the type descriptor.
 * The id of the type is the id of the keyword + 020 for every indirection
 */
length(t) {

	if (t<0)
		t += 020;
	if (t>=020)  /* array/pointer */
		return(4);
	switch(t) {

	case 0:
		return(4);

	case 1:
		return(1);

	case 2:
		return(4);

	case 3:
		return(8);

	case 4:  /* ??? what's this */
		return(4);

	}
	return(1024);
}

/* rounded length */
rlength(c) {
	auto l;

	return((l=length(c))==1? 4: l);
}


/* prints the number n in base b */
printn(n,b) {
	auto a;

	if(a=n/b) /* assignment, not test for equality */
		printn(a, b); /* recursive */
	putchar(n%b + '0');
}

putwrd(a) {
    printf("%d;", a);
}

cc_putchar(int c)
{
    putc(c, fout);
}

void cc_printf(char *fmt, ...)
{
	static char *s;
	auto *adx, x, c, *i;
	va_list arguments;

    va_start ( arguments, fmt);
loop:
	while((c = *fmt++) != '%') {
		if(c == '\0') {
		    va_end(arguments);
            return;
        }
		putchar(c);
	}
	switch (c = *fmt++) {

	case 'd': /* decimal */
	case 'o': /* octal */
	    x = va_arg(arguments, int);
		if(x < 0) {
			x = -x;
			if(x<0)  {	/* - infinity */
				if(c=='o')
					printf("100000");
				else
					printf("-32767");
				goto loop;
			}
			putchar('-');
		}
		printn(x, c=='o'?8:10);
		goto loop;

	case 's': /* string */
	    x = va_arg(arguments, int);
	    s=x;
		while(c = *s++) {
            putchar(c);
        }
		goto loop;

	case 'p':
		s =va_arg(arguments, int*);
		putchar('_');
		c = namsiz;
		while(c--)
			if(*s)
				putchar(*s++);
		goto loop;
	}
	putchar('%');
	fmt--;
	goto loop;
}

