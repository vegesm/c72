#ifndef LEGACY_CC_C1_H
#define LEGACY_CC_C1_H

#include <stdio.h>
#include <stdarg.h>
#include "config.h"

/* c10.c */
char *match(int*, int*, int);
void rcexpr(int*, int*, int);
int cexpr(int*, int*, int);
void pname(int *);
int dcalc(int *, int);
int notcompat(int,int );
void prins(int, int);
void printreg(int);
void printlreg(int);
int collcon(int*);
int isfloat(int *, char *s[]);

/* c11.c */
void cbranch(int[], int, int, int);
void branch(int,int,int);
void jump(int);
void label(int);
void popstk(int);
int length(int);
int rlength(int);
int getwrd();
void printn(int,int);
void cc_printf(char*,...);
void cc_putchar(int);
void error(char[]);
void error1(char[],int);
void error2(char[],int,int);

int *fixp(int*);

/* Globals */
extern int *regtab;
extern int *efftab;
extern int *cctab;
extern int *sptab;

extern int opdope[];
extern int ospace[];
extern int *baseptr;
extern const int nreg;
extern int isn;
extern const int namsiz;
extern FILE* fout;
extern int line;
extern int tmpfil;
extern int nerror;
extern int fltmod;

extern int instabcode[];
extern char *instabstr[];

#define printf cc_printf
#define putchar cc_putchar

#endif //LEGACY_CC_C1_H
