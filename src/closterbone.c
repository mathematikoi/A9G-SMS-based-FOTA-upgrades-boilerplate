#include "includes/closterbone.h"
#include "includes/sms.h"

static HANDLE closterbone_task_handle;
bool gprs_registration = false;

void closterbone_task(void *pData)
{
    API_Event_t *event = NULL;

    sms_init();

    while (1)
    {
        if (OS_WaitEvent(closterbone_task_handle, (void **)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            event_dispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void closterbonebase_Main(void)
{
    closterbone_task_handle = OS_CreateTask(closterbone_task,
                                            NULL, NULL, (2048 * 2), 0, 0, 0, "closterbone");
    OS_SetUserMainHandle(&closterbone_task_handle);
}