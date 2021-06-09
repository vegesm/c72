#include "c1.h"

/*
 * Tree is an unfixed pointer to the conditional expression.
 * lbl - label to jump to
 * cond - 0: jump if condition is false, 1: jump if condition is true
 */
void cbranch(int *tree, int lbl, int cond, int reg) {
	int l1, *origtree;

	if (tree==0)
		return;

	origtree=tree;
	tree=fixp(tree);
	switch(*tree) {

	/* & */
	case 47:
		if (cond) {
			cbranch(tree[3], l1=isn++, 0, reg);
			cbranch(tree[4], lbl, 1, reg);
			label(l1);
		} else {
			cbranch(tree[3], lbl, 0, reg);
			cbranch(tree[4], lbl, 0, reg);
		}
		return;

	/* | */
	case 48:
		if (cond) {
			cbranch(tree[3], lbl, 1, reg);
			cbranch(tree[4], lbl, 1, reg);
		} else {
			cbranch(tree[3], l1=isn++, 1, reg);
			cbranch(tree[4], lbl, 0, reg);
			label(l1);
		}
		return;

	/* ! */
	case 34:
		cbranch(tree[3], lbl, !cond, reg);
		return;
	}
	rcexpr(origtree, cctab, reg);
	branch(lbl, *tree, !cond);
}


void branch(int lbl, int op, int c) {
	if(op) {
		if((opdope[op]&04)==0)  /* conditional jump? */
			op = 61;

		printf("j");  /* instruction code of conditional jumps is the condition mnemonic */
		prins(op, c);
	} else
		printf("jmp");
	printf("\tl%d\n", lbl);
}

void jump(int lab) {
	printf("jmp\tl%d\n", lab);
}

void label(int l) {
	printf("l%d:", l);
}

/* Decreases the stack size, moving sp the required amounts. */
void popstk(int a) {

	switch(a) {

	case 0:
		return;

//	case 2:
//		printf("tst	(sp)+\n");
//		return;
//
//	case 4:
//		printf("cmp	(sp)+,(sp)+\n");
//		return;
	}
	printf("add	$%d,%esp\n", a);
}

int *fixp(int *p) {
    return (void*)p-(void*)baseptr+(void*)ospace;
}

int length(int t) {

	if (t<0)
		t += 020;
	if (t>=020)
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

	case 4:
		return(4);

	}
	return(1024);
}

/* rounded length */
int rlength(int c) {
	int l;

	return((l=length(c))==1? 4: l);
}

/* Get a number from the input file */
int getwrd() {
    int i;
    char c;

    scanf("%d", &i);
    c = getchar();
    if(c!=';') {
        error1("Unknown char id: %d", (int) c);
    }

    return i;
}

void printn(int n, int base) {
	int a;

	if(a= n / base) /* assignment, not test for equality */
		printn(a, base); /* recursive */
	putchar(n % base + '0');
}

void cc_putchar(int c)
{
    putc(c, fout);
    fflush(fout);
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
		    fflush(stdout);
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
		#ifdef WIN32
		putchar('_');
        #endif
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
	fout = f;
}

