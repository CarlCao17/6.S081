#include "kernel/types.h"
#include "user/user.h"
#include "user/utils.h"

int
Read(int fd, void * buf, int nbytes)
{
    int n;
    if ((n = read(fd, buf, nbytes)) < 0) {
        fprintf(2, "error: read failed\n");
        exit(1);
    }
    return n;
}

int
Write(int fd, const void* buf, int nbytes)
{
    int n;
    if ((n = write(fd, buf, nbytes)) < 0) {
        fprintf(2, "error: write failed\n");
        exit(1);
    } else if (n < nbytes) {
        fprintf(2, "error: write %d bytes, less than %d\n", n, nbytes);
        exit(1);
    }
    return n;
}


int
Fork(void)
{
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fatal: fork failed\n");
        exit(1);
    }
    return pid;
}