#ifndef __CBASETASK_H__
#define __CBASETASK_H__

#include "commonStd.h"
#include "commonOs.h"

typedef void (*TaskFunction_t)( void*);

class CBaseTask
{
protected:
    std::string mTaskName;
    uint32_t mStackSize;
    uint32_t mPriority;
    void* mParams;
    TaskFunction_t mTaskFunction;

    TaskHandle_t mPxCreatedTask;
public:
    CBaseTask(std::string taskName, uint32_t stackSize, uint32_t priority, void* params, TaskFunction_t mTaskFunction);
    ~CBaseTask();
    bool taskInit(void);
    bool taskRun(void);
    void taskDelete(void);
    void taskSuspend(void);
    void taskResume(void);
    void taskDelay(uint32_t delay);
};

#endif //__CBASETASK_H__