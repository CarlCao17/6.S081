#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int secs;

    if (argc < 2) {
        fprintf(2, "usage: sleep <seconds>\n");
        exit(1);
    }
    secs = atoi(argv[1]);

    sleep(secs);
    exit(0);
}