#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

#define ONLY_FILE 0
#define FILE_AND_DIR 1


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  int n = strlen(p);
  if(n >= DIRSIZ)
    return p;
  
  memmove(buf, p, n);
  buf[n] = 0;
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path, char *pattern, int find_mode)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    char *file_name;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return ;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return ;
    }

    file_name = fmtname(path);
    switch (st.type) {
        case T_FILE:
            if (strcmp(file_name, pattern) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            if (find_mode == FILE_AND_DIR && (strcmp(file_name, pattern) == 0)) {
                printf("%s\n", buf);
            }
        
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if ((de.inum == 0) || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                find(buf, pattern, find_mode);
            }
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find <path> <pattern>\n");
        exit(1);
    }
    // The Test left the check for DIR
    find(argv[1], argv[2], FILE_AND_DIR);
    exit(0);
}