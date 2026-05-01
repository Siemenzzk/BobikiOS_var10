#include "rtos_api.h"

extern TCB task_table[];
extern int task_count;
extern int current_task;
extern RCB resource_table[];
extern int resource_count;

TResource register_resource(int ceiling) {
    int id = resource_count;
    resource_table[id].id = id;
    resource_table[id].ceiling = ceiling;
    resource_table[id].is_locked = 0;
    resource_table[id].owner = -1;
    resource_count++;
    return id;
}

void GetResource(TResource res) {
    if (resource_table[res].is_locked) {
        return;
    }

    resource_table[res].is_locked = 1;
    resource_table[res].owner = current_task;

    if (task_table[current_task].priority < resource_table[res].ceiling) {
        task_table[current_task].priority = resource_table[res].ceiling;
    }
}

void ReleaseResource(TResource res) {
    if (!resource_table[res].is_locked ||
        resource_table[res].owner != current_task) {
        return;
    }

    resource_table[res].is_locked = 0;
    resource_table[res].owner = -1;

    task_table[current_task].priority = task_table[current_task].base_priority;

    dispatch();
}
