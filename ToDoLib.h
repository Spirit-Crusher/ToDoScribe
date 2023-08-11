#include <stdio.h>
#include <stdlib.h>

#define CHUNK 10

typedef struct Task
{
    int prioLvl;
    char* TaskName;
    struct Task* nextTask;
}Task;
