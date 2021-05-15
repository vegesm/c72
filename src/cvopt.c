#include <stdio.h>

#define getc cc_getc
getc();
flag();

peekc = 0;
nofloat = 0;
void main() {
/*
  converts code generation template .s files by replacing % part using the rules below:
	A1 -> A
	A2    B
	A     O
	B1    C
	B2    D
	BE    L
	BF    P
	C1    E
	C2    F
	F     G
	H     H
	R     I
	R1    J
    RL    Q
	S     K
	I     M
	M     N

		*	+1
		S	+2
		C	+4
		1	+8

	z  -> 4
	c     10
	a     14
	e     20
	n     63
	*	+0100
*/

	auto c,snlflg,nlflg,t,smode,m,ssmode,tempm;

	/*
	 * ssmode: next non % should output a data segment header
	 * tempm - in template mode: an upcoming '%' means a template definition and not a register
	 */

	smode = nlflg = snlflg = ssmode = 0;
	tempm = 1;
loop:
	c = getc();
	if(c!='%') tempm = 0;  /* if newline, it will be set to 1 later anyway, otherwise leaving template mode */
	if (c!='\n' & c!='\t') nlflg = 0;
	if (ssmode!=0 & c!='%') {
		ssmode = 0;
		printf(".data\n1: .ascii \"");
	}
	switch(c) {

	case '\0':
		printf(".text; .int 0\n");
		fflush(stdout);
		return;

	case ':':
		if (!smode)  /* convert into pointer */
			printf("=.+4; .int 0"); else  /* should be sizeof(int*) */
			putchar(':');
		goto loop;

	case 'A':
		if ((c=getc())=='1' | c=='2') {
			putchar(c+'A'-'1');
			goto loop;
		}
		putchar('O');
		peekc = c;
		goto loop;

	case 'B':
		switch (getc()) {

		case '1':
			putchar('C');
			goto loop;

		case '2':
			putchar('D');
			goto loop;

		case 'E':
			putchar('L');
			goto loop;

		case 'F':
			putchar('P');
			goto loop;
		}
		putchar('?');
		goto loop;

	case 'C':
		putchar(getc()+'E'-'1');
		goto loop;

	case 'F':
		putchar('G');
		goto subtre;

	case 'R':
	    c=getc();
		if (c== '1') {
        putchar('J');}
		else if (c=='L'){
            putchar('Q');
        } else {
			putchar('I');
			peekc = c;
		}
		goto loop;

	case 'H':
		putchar('H');
		goto subtre;

	case 'I':
		putchar('M');
		goto loop;

	case 'M':
		putchar('N');
		snlflg++;
		goto loop;

	case 'S':
		putchar('K');
subtre:
		snlflg = 1;
		t = 'A';
l1:
		switch (c=getc()) {

		case '*':
			t++;
			goto l1;

		case 'S':
			t += 2;
			goto l1;

		case 'C':
			t += 4;
			goto l1;

		case '1':
			t += 8;
			goto l1;
		}
		peekc = c;
		putchar(t);
		goto loop;

	case '#':
		if(getc()=='1')
			putchar('#'); else
			printf("\\\"");
		goto loop;

	case '%':
	    if(!tempm) {
	        putchar(c);
	        goto loop;
	    }
		if (smode)
			printf(".text;");
loop1:
		switch (c=getc()) {

		case 'a':
			m = 16;
			t = flag();
			goto pf;

		case ',':
			putchar(';');
			goto loop1;

		case 'i':
			m = 12;
			t = flag();
			goto pf;
		case 'z':
			m = 4;
			t = 0;
			goto pf;

		case 'c':
			t = flag();
			m = 8;
			goto pf;

		case 'e':
			t = flag();
			m = 20;
			goto pf;

		case 'n':
			t = flag();
			m = 63;
pf:
			if ((c=getc())=='*')
				m += 0100; else
				peekc = c;
			printf(".byte 0%o,0%o", m, t);
			goto loop1;

		case '\n':
			printf(";.int 1f\n");
			ssmode = 1;
			nlflg = 1;
			smode = 1;
			tempm = 1;
			goto loop;
		}
		putchar(c);
		goto loop1;

	case '\t':
		if (nlflg) {
			nlflg = 0;
			goto loop;
		}
		putchar('\t');
		goto loop;

	case '\n':
	    tempm = 1;
		if (!smode)  {  /* outside optree definition just emit new line */
			putchar('\n');
			goto loop;
		}
		if (nlflg) {  /* empty line, close off optree section */
			nlflg = 0;
			printf("\\0\"\n.text\n");
			smode = 0;
			goto loop;
		}
		if (!snlflg)
			printf("\\n");
		snlflg = 0;
		printf("\"\n.ascii \"");
		nlflg = 1;
		goto loop;
	}
	putchar(c);
	goto loop;
}

/* Gets the next character, ignores blocks of codes inside { } */
getc() {
	auto t, ifcnt;

	ifcnt = 0;
gc:
	if (peekc) {
		t = peekc;
		peekc = 0;
	} else
		t = getchar();
	if (t==0 | t==EOF)
		return(0);
	if (t=='{') {
		ifcnt++;
		t = getchar();
	}
	if (t=='}') {
		t = getc();
		if (--ifcnt==0)
			if (t=='\n')
				t = getc();
	}
	if (ifcnt & nofloat)
		goto gc;
	return(t);
}

flag() {
	auto c, f;

	f = 0;
l1:
	switch(c=getc()) {

	case 'w':  /* word */
		f = 1;
		goto l1;

	case 'i':
		f = 2;
		goto l1;

	case 'b':  /* byte */
		f = 3;
		goto l1;

	case 'f':  /* float */
		f = 4;
		goto l1;

	case 'd':  /* double */
		f = 5;
		goto l1;

	case 'p':  /* pointer? */
		f += 16;
		goto l1;
	}
	peekc = c;
	return(f);
}

