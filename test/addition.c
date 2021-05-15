main() {
    extern iarr;
    int iarr[];
    int a,b;
    char c1, c2, carr[4];

    iarr[2] = -10;
    carr[0] = 10;
    carr[1] = -10;
    carr[2] = 127;

    a=1;
    b=205;
    printf("3+4 = %d\n", 3+4);
    printf("a+4 = %d\n", a+4);
    printf("a+b = %d\n", a+b);

    printf("5+*iarr = %d\n", 5+*iarr);
    printf("5+iarr[1] = %d\n", 5+iarr[1]);
    printf("5+iarr[2] = %d\n", 5+iarr[2]);
    printf("5+iarr[3] = %d\n", 5+iarr[3]);

    c1=1;
    c2=127;
    printf("c1+4 = %d\n", c1+4);
    printf("c2+4 = %d\n", c2+4);
    printf("a+c1 = %d\n", a+c1);

    printf("5+*carr = %d\n", 5+*carr);
    printf("5+carr[1] = %d\n", 5+carr[1]);
    printf("5+carr[2] = %d\n", 5+carr[2]);
    printf("5+carr[3] = %d\n", 5+carr[3]);
}

iarr[] 0, 5, 10, 128;

