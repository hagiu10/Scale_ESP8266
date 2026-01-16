#include <rtos.h>

/** Constructor
 */
rtos::rtos() {
    _sysTickTimer = 0;
    _taskCount = 0;
    _taskList = new Task[MAX_TASKS];
#ifdef DEBUG
    Serial.printf("rtos::rtos Constructor. [%lu ms]\n", millis());
#endif
}
/** Destructor
 */
rtos::~rtos() {
    delete[] _taskList; // Free memory allocated for task list
#ifdef DEBUG
    Serial.printf("rtos::~rtos Destructor. [%lu ms]\n", millis());
#endif
}
/** Initialize the RTOS
 */
void rtos::init(void) {
    uint16_t timeWait = 100; // Wait time for the timer to initialize
    timer::init();
    timer::interrupt(_sysTickHandler_10us);
    delay(timeWait); // Allow time for the timer to initialize
#ifdef DEBUG
    Serial.printf("rtos::init Wait %d [ms]. [%lu ms]\n", timeWait, millis());
    Serial.printf("rtos::init Initialized. [%lu ms]\n", millis());
#endif
}
/** Add a task to the RTOS
 */
void rtos::addTask(const char taskName[],function_callback pfuncExec, u_long cycleTimeRun_us, uint8_t state) {
    rtos* rtosInstance = rtos::_getInstance();
    // Task count is between 0 and MAX_TASKS
    if (rtosInstance->_taskCount >= MAX_TASKS) {
        #ifdef DEBUG
            Serial.printf("rtos::addTask [error] Task count exceeded. [%lu ms]\n", millis());
        #endif
        return;
    }
    for(int i = 0; i < rtosInstance->_taskCount; i++) {
        // Check if task with the same name or function pointer already exists
        if(rtosInstance->_taskList[i].pfuncExec == nullptr) {
            continue; // Skip uninitialized tasks
            #ifdef DEBUG
                Serial.printf("rtos::addTask [warning] Skipping uninitialized task at index %d. [%lu ms]\n", i, millis());
            #endif
        }
        if (String(rtosInstance->_taskList[i].taskName).equals(String(taskName).c_str())) {
            #ifdef DEBUG
                Serial.printf("rtos::addTask [error] Task with taskName = %s already exists. [%lu ms]\n", String(taskName).c_str(), millis());
            #endif
            return;
        }
        if (rtosInstance->_taskList[i].pfuncExec == pfuncExec) {
            #ifdef DEBUG
                Serial.printf("rtos::addTask [error] Task with pfuncExec = %p already exists. [%lu ms]\n", pfuncExec, millis());
            #endif
            return;
        }
    }
    // Run time is between 1 ms and 100 ms
    if (cycleTimeRun_us > MAX_CYCLE_TIME) {
        #ifdef DEBUG
            Serial.printf("rtos::addTask [warning] Run time of task exceeded with %d us. [%lu ms]\n",cycleTimeRun_us - MAX_CYCLE_TIME, millis());
        #endif
        cycleTimeRun_us = MAX_CYCLE_TIME;
        
    }
    if (cycleTimeRun_us < MIN_CYCLE_TIME) {
         #ifdef DEBUG
            Serial.printf("rtos::addTask [warning] Run time of task is too short with %d us. [%lu ms]\n", MIN_CYCLE_TIME - cycleTimeRun_us, millis());
        #endif
        cycleTimeRun_us = MIN_CYCLE_TIME; 
    }
    // Add the task to the list
    rtosInstance->_taskList[rtosInstance->_taskCount].taskName = String(taskName);
    rtosInstance->_taskList[rtosInstance->_taskCount].pfuncExec = pfuncExec;
    rtosInstance->_taskList[rtosInstance->_taskCount].cycleTimeRun_us = cycleTimeRun_us;
    rtosInstance->_taskList[rtosInstance->_taskCount].state = state;
#ifdef DEBUG
    Serial.printf("rtos::addTask taskName = %s, pfuncExec = %p, cycleTimeRun_us = %lu, _taskCount = %d . [%lu ms]\n",
        rtosInstance->_taskList[rtosInstance->_taskCount].taskName.c_str(),
        rtosInstance->_taskList[rtosInstance->_taskCount].pfuncExec,  
        rtosInstance->_taskList[rtosInstance->_taskCount].cycleTimeRun_us,
        rtosInstance->_taskCount, 
        millis());
#endif
    // Increment the task count
    rtosInstance->_taskCount++;
}
/** Remove a task from the RTOS
 */
void rtos::removeTask(const char taskName[]) {
    rtos* rtosInstance = rtos::_getInstance();
    for (int i = 0; i < rtosInstance->_taskCount; i++) {
        if (String(rtosInstance->_taskList[i].taskName).equals(String(taskName).c_str())) {
            for (int j = i; j < rtosInstance->_taskCount - 1; j++) {
                rtosInstance->_taskList[j] = rtosInstance->_taskList[j + 1];
            }
            rtosInstance->_taskCount--;
            break;
        }
    }
#ifdef DEBUG
    Serial.printf("rtos::removeTask Task with taskName = %s removed. [%lu ms]\n", taskName, millis());
#endif
}
/** Change state of task
 * 
 */
void rtos::changeStateTask(String taskName, uint8_t state) {
    rtos* rtosInstance = rtos::_getInstance();
    for (int i = 0; i < rtosInstance->_taskCount; i++) {
        if (String(rtosInstance->_taskList[i].taskName).equals(taskName.c_str())) {
            rtosInstance->_taskList[i].state = state;
            #ifdef DEBUG
                Serial.printf("rtos::changeStateTask Task with taskName = %p changed state to %d. [%lu ms]\n", 
                    rtosInstance->_taskList[i].taskName, state, millis());
            #endif
            return;
        }
    }
    #ifdef DEBUG
        Serial.printf("rtos::changeStateTask [error] Task with taskName = %s not found. [%lu ms]\n", taskName.c_str(), millis());
    #endif
}
/** Execute the tasks in the RTOS
 */ 
void rtos::executeTasks() {
    rtos* rtosInstance = rtos::_getInstance();
    for (int i = 0; i < rtosInstance->_taskCount; i++) {
        if (rtosInstance->_taskList[i].state == TASK_READY) {
            rtosInstance->_taskList[i].pfuncExec();
            rtosInstance->_taskList[i].state = TASK_WAITING;
            #ifdef DEBUG
                //Serial.printf("rtos::executeTasks Task with pfuncExec = %p executed. [%lu ms]\n", rtosInstance->_taskList[i].pfuncExec, millis());
            #endif
        }
    }
}
/** System tick handler
 */
void rtos::_sysTickHandler_10us(void) {
    rtos* rtosInstance = rtos::_getInstance();
    if (rtosInstance == nullptr) {
        #ifdef DEBUG
            Serial.printf("rtos:: [error] RTOS instance is null. [%lu ms]\n", millis());
        #endif
        return;
    }
    rtosInstance->_sysTickTimer += INTERRUPT_PERIOD; // 10 us
    // Monitor and manage task states and execution times
    rtosInstance->_monitorTasks();
#ifdef DEBUG
    // It is not recommended to use Serial.print in an interrupt service routine
    // Serial.printf("rtos: System tick handler _sysTickTimer = %lu us\n", rtosInstance->_sysTickTimer);
#endif
}
/** Monitor and manage task states and execution times
 */
void rtos::_monitorTasks() {
    rtos* rtosInstance = rtos::_getInstance();
    // start from 1 to skip the idle task
    for (int i = 0; i < rtosInstance->_taskCount; i++) {
        if (rtosInstance->_taskList[i].state == TASK_BLOCKED) {
            continue;
        }
        if (rtosInstance->_taskList[i].state == TASK_READY) {
            continue;
        }
        if ((rtosInstance->_sysTickTimer % rtosInstance->_taskList[i].cycleTimeRun_us) == 0) {
            rtosInstance->_taskList[i].state = TASK_READY;
        }
    }
}
/** Get the rtosInstance of the RTOS
 */
rtos* rtos::_getInstance() {
    // Declare a static instance of the rtos class. This ensures that only one instance of the class exists.
    static rtos rtosInstance;
    // Return a pointer to the single instance of the rtos class.
    return &rtosInstance;
}
