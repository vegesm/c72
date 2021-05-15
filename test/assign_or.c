main() {
    extern iarr;
    int iarr[];
    int a,b;
    char c, d, carr[4];

    a = 5;
    printf("a | 3 = %d\n", a =| 3);
    printf("a = %d\n", a);

    a = 5; b =3;
    printf("a | b = %d\n", a =| b);
    printf("a = %d\n", a);

    printf("iarr[0] | b = %d\n", *iarr =| b);
    printf("iarr[0] = %d\n", iarr[0]);

    printf("iarr[1] | b = %d\n", iarr[1] =| b);
    printf("iarr[1] = %d\n", iarr[1]);

    c = 5;
    printf("c | 3 = %d\n", c =| 3);
    printf("c = %d\n", c);

    c = 5; d =3;
    printf("c | d = %d\n", c =| b);
    printf("c = %d\n", c);

    carr[0] = 5;
    carr[1] = -10;

    printf("carr[0] | d = %d\n", *carr =| d);
    printf("carr[0] = %d\n", carr[0]);

    printf("carr[0] | 128 = %d\n", *carr =| 128);
    printf("carr[0] = %d\n", carr[0]);
    printf("carr[1] = %d\n", carr[1]);

    printf("carr[1] | d = %d\n", carr[1] =| d);
    printf("carr[1] = %d\n", carr[1]);
}

iarr[] 0, 5, 10, 128;

