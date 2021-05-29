main () {
    int a;
    a=0;
    switch(a) {
        case 0:
            printf("test1 - 0\n");
        case 1:
            printf("test1 - 1\n");
        default:
            printf("test1 - default\n");
    }

    a=1;
    switch(a) {
        case 0:
            printf("test2 - 0\n");
            break;
        case 1:
            printf("test2 - 1\n");
            break;
        default:
            printf("test2 - default\n");
            break;
    }

    a=5;
    switch(a) {
        case 0:
            printf("test3 - 0\n");
        case 1:
            printf("test3 - 1\n");
        default:
            printf("test3 - default\n");
    }

    printf("end of program\n");
}