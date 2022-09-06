#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define N 128

void 
print_string_array(int fd, char * s[])
{
    int i;
    for (i = 0; s[i]; i++) {
        if (i > 0)
            fprintf(fd, " ");
        fprintf(fd, "%s", s[i]);
    }
    fprintf(fd, "\n");
}

int
main(int argc, char *argv[])
{
    int i, n;
    char *aargv[MAXARG];
    char *p, *buf;
    
    for (n = 1; argv[n]; n++) {
        aargv[n-1] = argv[n];
    }
    aargv[n-1] = 0;

    i = n - 1;
    buf = (char *)malloc(N);
    p = buf;
    while(read(0, p, 1)) {
        char pp = *p;
        if (pp != '\0' && pp != ' ' && pp != '\n') {
            p++;
            continue;
        }

        *p = '\0';
        aargv[i++] = buf;
        buf = (char *)malloc(N);
        p = buf;
        
        if (pp == '\n' || pp == '\0') {
            aargv[i] = 0; 
            if (fork() == 0) {
                if (exec(aargv[0], aargv) < 0) {
                    fprintf(2, "error: failed to exec ");
                    print_string_array(2, aargv);
                    exit(1);
                }
            } 
            if (pp == '\0')
                break;
            // pp == '\n'
            i = n - 1; 
        }
    }
    exit(0);
}