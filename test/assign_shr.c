main() {
    extern iarr;
    int iarr[];
    int a,b;
    char c, d, carr[4];

    a = 5;
    printf("a >> 2 = %d\n", a =>> 2);
    printf("a = %d\n", a);

    a = 128;
    printf("a >> 3 = %d\n", a =>> 3);
    printf("a = %d\n", a);

    a = 24; b =3;
    printf("a >> b = %d\n", a =>> b);
    printf("a = %d\n", a);
    printf("b = %d\n\n", b);

    printf("iarr[0] >> 3 = %d\n", *iarr =>> 3);
    printf("iarr[0] = %d\n", iarr[0]);
    printf("iarr[1] = %d\n", iarr[1]);

    iarr[0] = 1;
    printf("iarr[1] >> 3 = %d\n", iarr[1] =>> 3);
    printf("iarr[0] = %d\n", iarr[0]);
    printf("iarr[1] = %d\n\n", iarr[1]);

    c = 5;
    printf("c >> 2 = %d\n", c =>> 2);
    printf("c = %d\n", c);
    c = -128;
    printf("c >> 3 = %d\n", c =>> 3);
    printf("c = %d\n", c);

    c = 5; d =2;
    printf("c >> d = %d\n", c =>> d);
    printf("c = %d\n", c);

    c = -128; d =2;
    printf("c >> d = %d\n", c =>> d);
    printf("c = %d\n\n", c);

    carr[0] = 5;
    carr[1] = -10;

    printf("carr[0] >> d = %d\n", *carr =>> d);
    printf("carr[0] = %d\n", carr[0]);
    printf("carr[1] = %d\n", carr[1]);

    carr[0] = 5;
    printf("carr[0] >> 2 = %d\n", *carr =>> 2);
    printf("carr[0] = %d\n", carr[0]);
    printf("carr[1] = %d\n", carr[1]);

    carr[0] = 5;
    printf("carr[1] >> d = %d\n", carr[1] =>> d);
    printf("carr[0] = %d\n", carr[0]);
    printf("carr[1] = %d\n", carr[1]);
}

iarr[] 1, 8, 10, 128;

