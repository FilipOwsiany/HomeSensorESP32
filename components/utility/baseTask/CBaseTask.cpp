#include "CBaseTask.h"

#include "CLogger.h"

CBaseTask::CBaseTask(std::string taskName, uint32_t stackSize, uint32_t priority, void* params, TaskFunction_t taskFunction)
    : mTaskName(taskName), mStackSize(stackSize), mPriority(priority), mParams(params), mTaskFunction(taskFunction)
{
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Debug, "CBaseTask::CBaseTask()");
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::ImportantInfo, "Task Name: %s", mTaskName.c_str());
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::ImportantInfo, "Stack Size: %ld", mStackSize);
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::ImportantInfo, "Priority: %ld", mPriority);
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::ImportantInfo, "Params: %p", mParams);
}

CBaseTask::~CBaseTask()
{
    CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Debug, "CBaseTask::~CBaseTask()");
    if (mPxCreatedTask != NULL) 
    {
        vTaskDelete(mPxCreatedTask);
        mPxCreatedTask = NULL;
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Warning, "CBaseTask::~CBaseTask(): Task is not created");
    }
}

bool CBaseTask::taskInit(void)
{
    if (mTaskName.empty()) 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Error, "Task Name is empty");
        return false;
    }
    if (mStackSize == 0 || mStackSize < 256 || mStackSize > (16 * 1024)) 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Error, "Stack Size is invalid");
        return false;
    }
    if (mPriority == 0 || mPriority > 25) 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Error, "Priority is invalid");
        return false;
    }
    if (mTaskFunction == 0) 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Error, "Task Function is invalid");
        return false;
    }
    return true;
}

bool CBaseTask::taskRun(void)
{
    return xTaskCreate(mTaskFunction, mTaskName.c_str(), mStackSize, mParams, mPriority, &mPxCreatedTask) == pdPASS;
}

void CBaseTask::taskDelete(void)
{
    if (mPxCreatedTask != NULL) 
    {
        vTaskDelete(mPxCreatedTask);
        mPxCreatedTask = NULL;
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Warning, "CBaseTask::taskDelete(): Task is not created");
    }
}
void CBaseTask::taskSuspend(void)
{
    if (mPxCreatedTask != NULL) 
    {
        vTaskSuspend(mPxCreatedTask);
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Warning, "CBaseTask::taskSuspend(): Task is not created");
    }
}
void CBaseTask::taskResume(void)
{
    if (mPxCreatedTask != NULL) 
    {
        vTaskResume(mPxCreatedTask);
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Warning, "CBaseTask::taskResume(): Task is not created");
    }
}

void CBaseTask::taskDelay(uint32_t delay)
{
    if (delay > 0) 
    {
        vTaskDelay(pdMS_TO_TICKS(delay));
    }
    else 
    {
        CLogger::log(CLoggerModule::BaseTask, CLoggerLevel::Error, "CBaseTask::taskDelay(): Delay is invalid");
    }
}