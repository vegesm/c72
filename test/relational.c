main() {
    extern iarr[];
    int a, b;
    char c ,d, carr[4];

    if(2>1) {
        printf("test1 - ok\n");
    }

    a = 3;
    if(a>1) {
        printf("test2 - ok\n");
    }

    a = 3;
    b = -4;
    if(a>b) {
        printf("test3 - ok\n");
    }

    if(iarr[1]>1) {
        printf("test4 - ok\n");
    }

    if(iarr[1]>2+2) {
        printf("test5 - ok\n");
    }

    c = 6;
    d = -7;
    if(c>1) {
        printf("test6 - ok\n");
    }

    if(c>d) {
        printf("test7 - ok\n");
    }

    carr[0] = 2;
    carr[1] = -128;
    if(d>carr[1]) {
        printf("test8 - ok\n");
    }

    printf("eval - rel1: %d\n", a>=3);
    printf("eval - rel2: %d\n", a>=9);
}


iarr[] 1, 8, 10, 128;

