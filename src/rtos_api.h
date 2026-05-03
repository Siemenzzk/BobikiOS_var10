#ifndef RTOS_API_H
#define RTOS_API_H

#include <setjmp.h>

#define MAX_TASKS 32
#define MAX_RESOURCES 16
#define MAX_EVENTS 16

typedef enum {
    SUSPENDED,
    READY,
    RUNNING
} TTaskState;

typedef int TTask;
typedef int TResource;
typedef unsigned int TEventMask;

extern jmp_buf os_context;
extern TEventMask sys_event_mask;

// TASKS
typedef struct {
    int        id;
    int        priority;
    int        base_priority;
    TTaskState state;
    jmp_buf    context;
    void       (*func)();
    int        started;
    int        activation_order;
    TEventMask waiting_mask;
} TCB;

void ActivateTask(TTask task);
void TerminateTask(void);

void StartOS(TTask task);
void ShutdownOS(void);

void dispatch(void);

TTask register_task(void (*func)(), int priority);
int get_current_priority(void);

#define DeclareTask(TaskID)         \
    extern int TaskID##_priority;   \
    extern void TaskID##_func(void);\
    TTask TaskID

#define TASK(TaskID, prio)          \
    void TaskID##_func(void);       \
    int TaskID##_priority = prio;   \
    void TaskID##_func(void)

// RESOURCES
typedef struct {
    int id;
    int ceiling;
    int is_locked;
    int owner;
} RCB;

TResource register_resource(int ceiling);

void GetResource(TResource res);
void ReleaseResource(TResource res);

#define DeclareResource(ResourceID) \
    TResource ResourceID

// EVENTS
void DeclareSysEvent(TEventMask mask);
void SetSysEvent(TEventMask mask);
void GetSysEvent(TEventMask* event);
void WaitSysEvent(TEventMask mask);

#endif // RTOS_API_H
