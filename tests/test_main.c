#include "rtos_api.h"
#include <stdio.h>

DeclareTask(task1);
DeclareTask(task2);

#define EVENT_DATA_READY  0x01
#define EVENT_DONE        0x02

TASK(task1, 2)
{
    printf("task1: activating task2\n");
    ActivateTask(task2);
    printf("task1: waiting for EVENT_DATA_READY\n");
    WaitSysEvent(EVENT_DATA_READY);
    printf("task1: event received\n");
    TerminateTask();
}

TASK(task2, 1)
{
    printf("task2: setting EVENT_DATA_READY\n");
    SetSysEvent(EVENT_DATA_READY);
    printf("task2: done\n");
    TerminateTask();
}

int main(void) {
    task1 = register_task(task1_func, task1_priority);
    task2 = register_task(task2_func, task2_priority);
    StartOS(task1);
    printf("OS shutdown\n");
    return 0;
}
