main() {
    extern iarr;
    int iarr[];
    int p[];
    int a,b;
    char c, d, carr[4], cp[];

    a = 4;
    a++;
    printf("test1: %d\n", a);

    --a;
    printf("test2: %d\n", a);

    c = 15; c++;
    printf("test3: %d\n", c);

    c = -128; ++c;
    printf("test4: %d\n", c);

    iarr[2]++;
    printf("test5: %d %d %d\n", iarr[1], iarr[2], iarr[3]);

    p = iarr;
    p++;
    printf("test6: %d\n", *p);

    carr[0] = 3; carr[1] = 10; carr[2] = -128;
    carr[0]++;
    printf("test7: %d %d\n", carr[0], carr[1]);

    cp = carr+1;
    cp++;
    printf("test8: %d\n", *cp);
}

iarr[] 1, 8, 10, 128;

