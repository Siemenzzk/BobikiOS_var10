#include "rtos_api.h"
#include <stdio.h>

/* ТЕСТ 1: вытеснение по приоритету
   Ожидаем: task_high раньше task_low */

DeclareTask(task_low);
DeclareTask(task_high);

TASK(task_low, 1)
{
    printf("TEST1: task_low start\n");
    ActivateTask(task_high);
    printf("TEST1: task_low resumed after task_high\n");
    TerminateTask();
}

TASK(task_high, 2)
{
    printf("TEST1: task_high start\n");
    TerminateTask();
}

void test1(void) {
    printf("\n--- TEST 1: preemption by priority ---\n");
    task_low = register_task(task_low_func, task_low_priority);
    task_high = register_task(task_high_func, task_high_priority);
    StartOS(task_low);
}

/* ТЕСТ 2: равные приоритеты — порядок активации
   Ожидаем: task_a раньше task_b (активирована раньше) */

DeclareTask(task_a);
DeclareTask(task_b);

TASK(task_a, 2)
{
    printf("TEST2: task_a start\n");
    TerminateTask();
}

TASK(task_b, 2)
{
    printf("TEST2: task_b start\n");
    TerminateTask();
}

DeclareTask(task_starter);

TASK(task_starter, 1)
{
    ActivateTask(task_a);
    ActivateTask(task_b);
    TerminateTask();
}

void test2(void) {
    printf("\n--- TEST 2: equal priority - activation order ---\n");
    task_starter = register_task(task_starter_func, task_starter_priority);
    task_a = register_task(task_a_func, task_a_priority);
    task_b = register_task(task_b_func, task_b_priority);
    StartOS(task_starter);
}

/* ТЕСТ 3: HLP — захват ресурса предотвращает вытеснение
   Ожидаем: task_med не вытесняет task_worker пока та держит ресурс */

DeclareTask(task_worker);
DeclareTask(task_med);
DeclareTask(task_top);
DeclareResource(res1);

TASK(task_worker, 1)
{
    printf("TEST3: task_worker start\n");
    GetResource(res1);
    printf("TEST3: task_worker got resource, priority = %d\n", get_current_priority());
    ActivateTask(task_med);
    ActivateTask(task_top);
    printf("TEST3: task_worker still running (HLP working)\n");
    ReleaseResource(res1);
    printf("TEST3: task_worker released resource\n");
    TerminateTask();
}

TASK(task_med, 2)
{
    printf("TEST3: task_med start\n");
    TerminateTask();
}

TASK(task_top, 3)
{
    printf("TEST3: task_top start\n");
    TerminateTask();
}

void test3(void) {
    printf("\n--- TEST 3: HLP resource protection ---\n");
    task_worker = register_task(task_worker_func, task_worker_priority);
    task_med = register_task(task_med_func, task_med_priority);
    task_top = register_task(task_top_func, task_top_priority);
    res1 = register_resource(3);
    StartOS(task_worker);
}

/* ТЕСТ 4: вложенные ресурсы
   Ожидаем: после освобождения res2 приоритет падает до ceiling res1,
   после освобождения res1 — до базового */

DeclareTask(task_nested);
DeclareTask(task_nested_high);
DeclareResource(res2);
DeclareResource(res3);

TASK(task_nested, 1)
{
    printf("TEST4: task_nested start, base priority = 1\n");
    GetResource(res2);
    printf("TEST4: got res2, priority = %d\n", get_current_priority());
    GetResource(res3);
    printf("TEST4: got res3, priority = %d\n", get_current_priority());
    ActivateTask(task_nested_high);
    printf("TEST4: task_nested still running (holds res3 ceiling=5)\n");
    ReleaseResource(res3);
    printf("TEST4: released res3, priority = %d\n", get_current_priority());
    ReleaseResource(res2);
    printf("TEST4: released res2, priority = %d\n", get_current_priority());
    TerminateTask();
}

TASK(task_nested_high, 4)
{
    printf("TEST4: task_nested_high start\n");
    TerminateTask();
}

void test4(void) {
    printf("\n--- TEST 4: nested resources ---\n");
    task_nested = register_task(task_nested_func, task_nested_priority);
    task_nested_high = register_task(task_nested_high_func, task_nested_high_priority);
    res2 = register_resource(3);
    res3 = register_resource(5);
    StartOS(task_nested);
}

/* ТЕСТ 5: системные события
   Ожидаем: task_waiter уходит в SUSPENDED,
   task_setter устанавливает событие,
   task_waiter просыпается */

DeclareTask(task_waiter);
DeclareTask(task_setter);

#define EVENT_READY 0x01

TASK(task_waiter, 2)
{
    printf("TEST5: task_waiter activating task_setter\n");
    ActivateTask(task_setter);
    printf("TEST5: task_waiter waiting for EVENT_READY\n");
    WaitSysEvent(EVENT_READY);
    printf("TEST5: task_waiter received event\n");
    TerminateTask();
}

TASK(task_setter, 1)
{
    printf("TEST5: task_setter setting EVENT_READY\n");
    SetSysEvent(EVENT_READY);
    printf("TEST5: task_setter done\n");
    TerminateTask();
}

void test5(void) {
    printf("\n--- TEST 5: system events ---\n");
    task_waiter = register_task(task_waiter_func, task_waiter_priority);
    task_setter = register_task(task_setter_func, task_setter_priority);
    StartOS(task_waiter);
}

int main(void) {
    test1();
    test2();
    test3();
    test4();
    test5();
    printf("\nAll tests done\n");
    return 0;
}
