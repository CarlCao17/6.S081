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
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int
find(char *path, char *pattern, int find_mode)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    char *file_name;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return 1;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return 1;
    }

    file_name = fmtname(path);
    printf("file name: %s\n", file_name);
    switch (st.type) {
        case T_FILE:
            if (strcmp(file_name, pattern) != 0) {
                return 1;
            }
            printf("%s\n", path);
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            if (find_mode == FILE_AND_DIR && (strcmp(file_name, pattern) == 0)) {
                printf("%s\n", buf);
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

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
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find <path> <pattern>\n");
        exit(1);
    }
    int res = find(argv[1], argv[2], ONLY_FILE);
    if (res > 0) {
        printf("find: cannot find %s in path %s\n", argv[2], argv[1]);
        exit(1);
    }
    exit(0);
}