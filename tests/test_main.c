#include "rtos_api.h"
#include <stdio.h>

DeclareTask(task1);
DeclareTask(task2);
DeclareTask(task3);
DeclareResource(res1);

TASK(task1, 3)
{
    printf("task1: start\n");
    TerminateTask();
}

TASK(task2, 2)
{
    printf("task2: start\n");
    TerminateTask();
}

TASK(task3, 1)
{
    printf("task3: start, getting resource\n");
    GetResource(res1);
    printf("task3: inside critical section, priority raised to %d\n",
        get_current_priority());
    ActivateTask(task1);
    ActivateTask(task2);
    printf("task3: still running despite task1 and task2 activated\n");
    ReleaseResource(res1);
    printf("task3: resource released\n");
    TerminateTask();
}

int main(void) {
    task1 = register_task(task1_func, task1_priority);
    task2 = register_task(task2_func, task2_priority);
    task3 = register_task(task3_func, task3_priority);
    
    res1 = register_resource(3);
    
    StartOS(task3);
    printf("OS shutdown\n");
    return 0;
}
