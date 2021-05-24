main() {
    extern iarr;
    int iarr[];
    int a,b;
    char c, d, carr[4];

    a = 4;
    printf("test1: %d\n", a);

    c = 15;
    printf("test2: %d\n", c);

    c = -128;
    printf("test3: %d\n", c);

    iarr[2] = 12345;
    printf("test4: %d %d %d\n", iarr[1], iarr[2], iarr[3]);

    carr[0] = 3;
    printf("test5: %d\n", carr[0]);
}

iarr[] 1, 8, 10, 128;

