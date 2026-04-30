#ifndef RTOS_API_H
#define RTOS_API_H

#include <setjmp.h>

#define MAX_TASKS 32
#define MAX_RESOURCES 16
#define MAX_EVENTS 16

// состо€ни€ задачи
typedef enum {
    SUSPENDED,
    READY,
    RUNNING
} TTaskState;

// идентификатор задачи Ч индекс в таблице задач
typedef int TTask;

// идентификатор ресурса Ч индекс в таблице ресурсов
typedef int TResource;

// маска событий Ч каждый бит соответствует одному событию
typedef unsigned int TEventMask;

typedef struct {
    int        id;
    int        priority;
    TTaskState state;
    jmp_buf    context;
    void       (*func)();
    int        started;
    int        activation_order;
} TCB;

// управление задачами
void ActivateTask(TTask task);
void TerminateTask(void);

// управление ќ—
void StartOS(TTask task);
void ShutdownOS(void);

// внутренний диспетчер
void dispatch(void);

// макросы
#define DeclareTask(TaskID) \
    extern TCB TaskID##_tcb; \
    TTask TaskID;

#define TASK(TaskID, priority)           \
    void TaskID##_func();                \
    TCB TaskID##_tcb = {                 \
        .priority = priority,            \
        .state    = SUSPENDED,           \
        .func     = TaskID##_func,       \
        .started  = 0,                   \
        .activation_order = -1           \
    };                                   \
    void TaskID##_func()

#endif // RTOS_API_H
