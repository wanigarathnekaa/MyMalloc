#include <stdio.h>
#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

void *MyMalloc(size_t size);
void MyFree(void *ptr);
#endif