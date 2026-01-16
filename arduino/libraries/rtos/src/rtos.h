#ifndef RTOS_H
#define RTOS_H

#include <commonLibs.h>
#include <timer.h>

#ifndef u_long
#define u_long unsigned long
#endif

#define MAX_TASKS 10U
#define INTERRUPT_PERIOD 10U // 10 us
#define MAX_CYCLE_TIME 1000000UL // 1000 ms
#define MIN_CYCLE_TIME 1000U // 1ms

// Function pointer
typedef void (*function_callback)  ();
// Task structure
typedef struct {
  String taskName;
  function_callback pfuncExec;
  u_long cycleTimeRun_us;
  uint8_t state;
} Task;
// Enumeration for task states
enum taskState {
  TASK_WAITING,
  TASK_READY,
  TASK_BLOCKED,
  TASK_STATE_COUNT
};
// Function prototypes
class rtos: private timer{
  private:
    u_long _sysTickTimer;
    uint8_t _taskCount;
    Task* _taskList;
    static void _sysTickHandler_10us(void);
    static rtos* _getInstance(void);
    void _monitorTasks(void);
  public:
    rtos();
    ~rtos();
    static void init(void);
    static void addTask(const char taskName[],
                function_callback pfuncExec, 
                u_long cycleTimeRun_us, 
                uint8_t state = TASK_BLOCKED);
    static void removeTask(const char taskName[]);
    static void changeStateTask(String taskName, uint8_t state);
    static void executeTasks(void);  
};
#endif // RTOS_H