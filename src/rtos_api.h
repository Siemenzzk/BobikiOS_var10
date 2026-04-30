#ifndef RTOS_API_H
#define RTOS_API_H

#include <setjmp.h>

#define MAX_TASKS 32
#define MAX_RESOURCES 16
#define MAX_EVENTS 16

// состояния задачи
typedef enum {
    SUSPENDED,
    READY,
    RUNNING
} TTaskState;

// идентификатор задачи — индекс в таблице задач
typedef int TTask;

// идентификатор ресурса — индекс в таблице ресурсов
typedef int TResource;

// маска событий — каждый бит соответствует одному событию
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

// управление ОС
void StartOS(TTask task);
void ShutdownOS(void);

// внутренний диспетчер
void dispatch(void);

// регистрация задачи
TTask register_task(void (*func)(), int priority);

// макросы
#define DeclareTask(TaskID)         \
    extern int TaskID##_priority;   \
    extern void TaskID##_func(void);\
    TTask TaskID

#define TASK(TaskID, prio)          \
    void TaskID##_func(void);       \
    int TaskID##_priority = prio;   \
    void TaskID##_func(void)

#endif // RTOS_API_H
