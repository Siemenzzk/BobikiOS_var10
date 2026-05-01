#include "rtos_api.h"


TCB task_table[MAX_TASKS];

int task_count = 0;

int current_task = -1;

int activation_counter = 0;

int os_running = 0;
