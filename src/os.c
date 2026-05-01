#include "rtos_api.h"

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
