main () {
    int a[1];

    goto skip1;
    printf("test1 - fail\n");
    skip1:
        printf("test1 - ok\n");

    goto (1 ? skip2 : 0);
    printf("test2 - fail\n");
    skip2:
        printf("test2 - ok\n");

    a[0] = skip3;
    goto a[0];
    printf("test3 - fail\n");
    skip3:
        printf("test3 - ok\n");
}