#include "rtos_api.h"
#include <stdio.h>

extern TCB task_table[];
extern int task_count;
extern int current_task;
extern int activation_counter;
extern int os_running;

void StartOS(TTask task) {
    if (os_running) {
        return;
    }

    os_running = 1;

    task_table[task].state = READY;
    task_table[task].activation_order = activation_counter++;

    if (setjmp(os_context) == 0) {
        dispatch();
    }
}

void ShutdownOS(void) {
    os_running = 0;
    current_task = -1;
    longjmp(os_context, 1);
}

void DeclareSysEvent(TEventMask mask) {
    (void)mask;
}

void SetSysEvent(TEventMask mask) {
    sys_event_mask |= mask;

    for (int i = 0; i < task_count; i++) {
        if (task_table[i].state == SUSPENDED &&
            task_table[i].waiting_mask & mask) {
            task_table[i].waiting_mask = 0;
            task_table[i].state = READY;
            task_table[i].activation_order = activation_counter++;
        }
    }

    dispatch();
}

void GetSysEvent(TEventMask* event) {
    *event = sys_event_mask;
}

void WaitSysEvent(TEventMask mask) {
    if (sys_event_mask & mask) {
        sys_event_mask &= ~mask;
        return;
    }

    task_table[current_task].waiting_mask = mask;
    task_table[current_task].state = SUSPENDED;

    dispatch();

    sys_event_mask &= ~mask;
}
