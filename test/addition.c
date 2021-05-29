main() {
    extern iarr;
    int iarr[];
    int a,b, tmp;
    char c1, c2, carr[4];

    iarr[2] = -10;
    carr[0] = 10;
    carr[1] = -10;
    carr[2] = 127;
    carr[3] = 0;

    a=1;
    b=205;
    tmp = 3+4; printf("3+4 = %d\n", tmp);
    tmp = a+4; printf("a+4 = %d\n", tmp);
    tmp = a+b; printf("a+b = %d\n", tmp);

    tmp = 5+*iarr; printf("5+*iarr = %d\n", tmp);
    tmp = 5+iarr[1]; printf("5+iarr[1] = %d\n", tmp);
    tmp = 5+iarr[2]; printf("5+iarr[2] = %d\n", tmp);
    tmp = 5+iarr[3]; printf("5+iarr[3] = %d\n", tmp);

    c1=1;
    c2=127;
    tmp = c1+4; printf("c1+4 = %d\n", tmp);
    tmp = c2+4; printf("c2+4 = %d\n", tmp);
    tmp = a+c1; printf("a+c1 = %d\n", tmp);

    tmp = 5+*carr; printf("5+*carr = %d\n", tmp);
    tmp = 5+carr[1]; printf("5+carr[1] = %d\n", tmp);
    tmp = 5+carr[2]; printf("5+carr[2] = %d\n", tmp);
    tmp = 5+carr[3]; printf("5+carr[3] = %d\n", tmp);
}

iarr[] 0, 5, 10, 128;

