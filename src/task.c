#include "rtos_api.h"

extern TCB task_table[];
extern int task_count;
extern int current_task;
extern int activation_counter;
extern int os_running;

int find_next_task(void) {
	int best = -1;
	
	for (int i = 0; i < task_count; i++) {
		
		if (task_table[i].state != READY) {
			continue;
		}

		if (best == -1) {
			best = i;
			continue;
		}

		if (task_table[i].priority > task_table[best].priority) {
			best = i;
			continue;
		}

		if (task_table[i].priority == task_table[best].priority &&
			task_table[i].activation_order < task_table[best].activation_order) {
			best = i;
		}
	}

	return best;
}

void dispatch(void) {

	if (current_task != -1 && task_table[current_task].state == RUNNING) {
		task_table[current_task].state = READY;
	}

	int next = find_next_task();

	if (next == -1) {
		ShutdownOS();
		return;
	}

	if (current_task != -1) {
		if (setjmp(task_table[current_task].context) != 0) {
			return;
		}
	}

	current_task = next;
	task_table[current_task].state = RUNNING;

	if (task_table[current_task].started == 0) {
		task_table[current_task].started = 1;
		task_table[current_task].func();
	}
	else {
		longjmp(task_table[current_task].context, 1);
	}
}

void ActivateTask(TTask task) {
	if (task_table[task].state != SUSPENDED) {
		return;
	}

	task_table[task].state = READY;
	task_table[task].activation_order = activation_counter++;

	dispatch();
}

void TerminateTask(void) {
	task_table[current_task].state = SUSPENDED;
	task_table[current_task].started = 0;

	task_table[current_task].activation_order = -1;

	current_task = -1;

	dispatch();
}

TTask register_task(void (*func)(), int priority) {
	int id = task_count;
	task_table[id].id = id;
	task_table[id].priority = priority;
	task_table[id].base_priority = priority;
	task_table[id].state = SUSPENDED;
	task_table[id].func = func;
	task_table[id].started = 0;
	task_table[id].activation_order = -1;
	task_table[id].waiting_mask = 0;
	task_count++;
	return id;
}

int get_current_priority(void) {
	return task_table[current_task].priority;
}
