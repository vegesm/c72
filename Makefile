CC = gcc
CFLAGS =-Wno-implicit-int -Wno-int-conversion -m32

c0_src = $(wildcard src/c0*.c)
c1_src = $(wildcard src/c1*.c)

c1_tabs = $(wildcard src/*tab.s)
c1_tabs_converted = $(c1_tabs:%tab.s=%tab_conv.s)

all: c0 c1

c0: $(c0_src) src/c0.h
	$(CC) $(CFLAGS) -o c0 $(c0_src)

c1: $(c1_src) $(c1_tabs_converted) src/c1.h
	$(CC) $(CFLAGS) -D __DIR__=\"`realpath src`\" -o c1 $(c1_src) $(c1_tabs_converted)

cvopt: src/cvopt.c
	$(CC) $(CFLAGS) -o cvopt src/cvopt.c

%tab_conv.s: %tab.s cvopt
	./cvtab.sh $< > $@

