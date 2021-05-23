main() {
    int a, b
    ;
    a = 1;
    if(a) {
        printf("test1 - a is nonzero\n");
    }

    a = 0;
    if(a) {
        printf("test2 - a is nonzero\n");
    }

    a = 0;
    if(!a) {
        printf("test3 - a is zero\n");
    }

    a = 0;
    if(a) {
        printf("test4 - if called\n");
    } else {
        printf("test4 - else called\n");
    }

    a = 0;
    if(a & printf("and short circuit fail\n"));

    a = 1;
    if(a | printf("or short circuit fail\n"));

    a = 0;
    b = 1;
    if(a | b) {
        printf("test5 - a | b\n");
    }

    a = 0;
    b = 0;
    if(a | b) {
        printf("test6 - a | b\n");
    }

    a = 1;
    b = 1;
    if(a & b) {
        printf("test7 - a | b\n");
    }

    a=6;
    b=5;
    if (a - b) {
        printf("test8 - ok\n");
    }

    b=6;
    if (a - b) {
        printf("test9 - fail\n");
    } else {
        printf("test9 - ok\n");
    }

    a=6;
    b=5;
    if (a % b) {
        printf("test10 - ok\n");
    }

    b=3;
    if (a % b) {
        printf("test11 - fail\n");
    } else {
        printf("test11 - ok\n");
    }
}

iarr[] 1, 8, 10, 128;

