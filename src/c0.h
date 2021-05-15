#include <stdio.h>
#include "config.h"
//#define DEBUG

/* c00.c */
setup();
init(char[], int);
int *lookup();
symbol();
subseq(int,int,int);
getstr();
getcc();
mapch(int);
tree();
void declare(int);

/* c01.c */
void build(int);
int *convert(int[], int, int, int);
chkw(int[]);
lintyp(int);
error(char[]);
error1(char[],int);
error2(char[],int,int);
int *block(int, ...);
chklval(int[]);
notcompat(int, int);
max(int, int);

/* c02.c */
void function(char[]);
void extdef();
void statement();
pexpr();
pswitch();
blkhed();
blkend();
errflush(int);
void declist();
easystmt();
branch(int);

/* c03.c */
jumpc(int[], int, int);
rcexpr(int[], int);
jump(int);
label(int);
retseq();
slabel();
void setstk(int);
defvec();
defstat(int[]);

length(int);
rlength(int);
putwrd(int);
printn(int,int);
void cc_printf(char[], ...);
cc_putchar(int);

/* globals */
extern ossiz;
extern ospace[];
extern regtab;
extern efftab;
extern cctab;
extern sptab;
extern symbuf[];
extern pssiz;
extern namsiz;
extern nwps;
extern hshused;
extern hshsiz;
extern hshlen;
extern hshtab[];
extern *space;
extern *cp;
extern cmsiz;
extern cmst[];
extern ctyp;
extern isn;
extern swsiz;
extern swtab[];
extern *swp;
extern contlab;
extern brklab;
extern deflab;
extern nreg;
extern maprel[];
extern nauto;
extern stack;
extern peeksym;
extern peekc;
extern eof;
extern line;
extern int *csym;
extern cval;
extern ncpw;
extern nerror;
extern FILE *fout;
extern int *paraml;
extern int *parame;
extern tmpfil;

/* code tables */
extern char ctab[];
extern int opdope[];
extern int cvtab[];
#define printf cc_printf
#define putchar cc_putchar
