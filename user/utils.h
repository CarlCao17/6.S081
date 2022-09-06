#include "kernel/types.h"
#include "user/user.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

// utils.c
int Fork(void);
int Read(int, void*, int);
int Write(int, const void*, int);