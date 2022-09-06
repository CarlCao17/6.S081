#include "user/utils.h"

void primes(int left[2]) {
    int n, prime;
    int pid = getpid();
    int right[2];
    BOOL is_first = TRUE;

    close(left[1]);

    if (Read(left[0], &prime, 4) < 4) {
        fprintf(2, "error: process <%d> failed to read first number from left\n", pid);
        exit(1);
    }
    printf("prime %d\n", prime);
    
    while (Read(left[0], &n, 4) == 4) {
        if (n % prime != 0) {
            if (is_first == TRUE) {
                is_first = FALSE;
                pipe(right);
                if (Fork() == 0) {
                    primes(right);
                    exit(0);
                } else {
                    close(right[0]);
                }
            }
            Write(right[1], &n, 4);
        }
    }
    close(left[0]);
    if (is_first == FALSE) {
        close(right[1]);
    }
}

int
main()
{
    int i;
    int p[2];

    pipe(p);
    if (Fork() == 0) {
        primes(p);
        exit(0);
    }
    close(p[0]);
    for (i = 2; i < 36; i++) {
        Write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
    exit(0);
}
