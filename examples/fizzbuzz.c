main() {
    auto n, i;
    n = 20;

    i = 1;  /* no for loops */
    while (i <= n) {
        if (i % 15 == 0) {
            printf("fizzbuzz\n");
        } else if (i % 3 == 0) {
            printf("fizz\n");
        } else if (i % 5 == 0) {
            printf("buzz\n");
        } else {
            printf("%d\n", i);
        }
        i++;
    }

}