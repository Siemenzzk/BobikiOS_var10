#include "rtos_api.h"
#include <stdio.h>

DeclareTask(task1);
DeclareTask(task2);

TASK(task1, 1)
{
    printf("task1: start\n");
    ActivateTask(task2);
    printf("task1: after activating task2\n");
    TerminateTask();
}

TASK(task2, 2)
{
    printf("task2: start\n");
    TerminateTask();
}

int main(void) {
    task1 = register_task(task1_func, task1_priority);
    task2 = register_task(task2_func, task2_priority);
    StartOS(task1);
    printf("OS shutdown\n");
    return 0;
}
