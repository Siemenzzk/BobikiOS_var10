#include "rtos_api.h"
#include <stdio.h>

DeclareTask(task1);
DeclareTask(task2);
DeclareTask(task3);
DeclareResource(res1);
DeclareResource(res2);

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
    printf("task3: start\n");
    GetResource(res1);
    printf("task3: got res1, priority = %d\n", get_current_priority());
    ActivateTask(task2);
    printf("task3: activated task2, still running (HLP working)\n");
    ActivateTask(task1);
    printf("task3: activated task1, still running (HLP working)\n");
    ReleaseResource(res1);
    printf("task3: released res1\n");
    TerminateTask();
}

int main(void) {
    task1 = register_task(task1_func, task1_priority);
    task2 = register_task(task2_func, task2_priority);
    task3 = register_task(task3_func, task3_priority);
    res1 = register_resource(3);  /* ceiling = max(3,1) = 3 */
    res2 = register_resource(5);
    
    StartOS(task3);
    printf("OS shutdown\n");
    return 0;
}
