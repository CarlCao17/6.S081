#include "kernel/types.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    int sent[2], rcv[2];
    int child;

    pipe(sent);
    pipe(rcv);

    if ((child = fork()) < 0) {
        fprintf(2, "error: fork\n");
        exit(1);
    } else if (child == 0) {
        char t;
        int pid = getpid();

        close(sent[1]);
        close(rcv[0]);

        if (read(sent[0], &t, 1) != 1) {
            fprintf(2, "error: failed to read a byte from parent, pid = %d\n", pid);
            exit(1);
        }
        fprintf(1, "%d: received ping\n", pid);
        if (write(rcv[1], &t, 1) < 1) {
            fprintf(2, "error: failed to write a byte to parent, pid = %d\n", pid);
            exit(1);
        }

        close(sent[0]);
        close(rcv[1]);
    } else {
        char t = 'p', rt;
        int pid = getpid();

        close(sent[0]);
        close(rcv[1]);

        if (write(sent[1], &t, 1) < 1) {
            fprintf(2, "error: failed to send byte to child, pid = %d", pid);
            exit(1);
        }
        if ((read(rcv[0], &rt, 1) != 1) || (rt != t)) {
            fprintf(2, "error: failed to receive byte from child, pid = %d", pid);
            exit(1);
        }
        fprintf(1, "%d: received pong\n", pid);

        close(sent[1]);
        close(rcv[0]);
    }
       exit(0);
}