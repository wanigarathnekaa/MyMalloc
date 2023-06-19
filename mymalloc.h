/*
    SCS1214 - Operating Systems I
    2100071 - K.A.A.Wanigarathne
    
    
    My implementation of malloc() and free(). MyMalloc() and MyFree() have similar
    functionalities to malloc() and free() but they are not identical. As an Example, 
    Real malloc() doesn't print anything upon failed to allocate Memory. For the sake of
    Simplicity and Easier for understanding, I implemented in that way.
    
    Here, for my implementation I have used First-fit allocation which may not be the
    optimal solution for each and every problem.
    
    In this implementation, I made a concerted effort to track free memory only.

*/

 

#include <stdio.h>
#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

void *MyMalloc(size_t size);
void MyFree(void *ptr);
#endif
