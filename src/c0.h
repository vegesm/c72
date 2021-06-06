#include <stdio.h>
#include "config.h"
//#define DEBUG

/* c00.c */
void init(char[], int);
int *lookup(void);
int symbol(void);
int subseq(int, int, int);
int getstr(void);
int getcc(void);
int mapch(int);
int tree(void);
void declare(int);

/* c01.c */
void build(int);
int *convert(int[], int, int, int);
void chkw(int[]);
int lintyp(int);
void error(char[]);
void error1(char[],int);
void error2(char[],int,int);
int *block(int, ...);
void chklval(int[]);
int max(int, int);

/* c02.c */
void function(char[]);
void extdef(void);
void statement(int);
int* pexpr(void);
void pswitch(void);
void blkhed(void);
void blkend(void);
void errflush(int);
void declist(void);
int easystmt(void);

/* c03.c */
void jumpc(int[], int, int);
void rcexpr(int[], int);
void jump(int);
void label(int);
void retseq(void);
void slabel(void);
void setstk(int);
void defvec(void);
void defstat(int[]);

int length(int);
int rlength(int);
void putwrd(int);
void printn(int,int);
void cc_printf(char[], ...);
void cc_putchar(int);

/* globals */
extern int ossiz;
extern int ospace[];
extern const int regtab;
extern const int efftab;
extern const int cctab;
extern const int sptab;
extern int symbuf[];
extern int const pssiz;
extern int const namsiz;
extern int const nwps;
extern int hshused;
extern int const hshsiz;
extern int const hshlen;
extern int hshtab[];
extern int *space;
extern int *cp;
extern const int cmsiz;
extern int cmst[];
extern int ctyp;
extern int isn;
extern const int swsiz;
extern int swtab[];
extern int *swp;
extern int contlab;
extern int brklab;
extern int deflab;
extern const int nreg;
extern int maprel[];
extern int stack;
extern int peeksym;
extern int peekc;
extern int eof;
extern int line;
extern int *csym;
extern int cval;
extern const int ncpw;
extern int nerror;
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
