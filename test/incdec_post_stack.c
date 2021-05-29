destr() {
    3/3;
    return (6);
}

deref(testid, p)
int testid;
char p[]; {
    printf("test%d: %d\n", testid, *p);
};

main() {
    extern iarr, deref;
    int iarr[];
    int p[];
    int a,b;
    char c, d, carr[4], cp[];

    a = 4;
    printf("test1: %d\n", a++);
    printf("test1: %d\n", a);

    printf("test2: %d\n", a--);
    printf("test2: %d\n", a);

    c = 15;
    printf("test3: %d\n", c++);
    printf("test3: %d\n", c);

    c = -128;
    printf("test4: %d\n", c++);
    printf("test4: %d\n", c);

    printf("test5: %d\n", iarr[2]++);
    printf("test5: %d %d %d\n", iarr[1], iarr[2], iarr[3]);

    p = iarr;
    deref(6, p++);
    printf("test6: %d\n", *p);

    carr[0] = 3; carr[1] = 10; carr[2] = -128;
    printf("test7: %d\n", carr[0]++);
    printf("test7: %d %d\n", carr[0], carr[1]);

    cp = carr;
    deref(8, cp++);
    printf("test8: %d\n", *cp);
}

iarr[] 1, 8, 10, 128;

