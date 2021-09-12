#include "includes/sms.h"
#include "includes/network.h"

void event_dispatch(API_Event_t *event)
{
    switch (event->id)
    {
    case API_EVENT_ID_SMS_RECEIVED:
    {

        SMS_Storage_Info_t storageInfo;
        SMS_GetStorageInfo(&storageInfo, SMS_STORAGE_SIM_CARD);
        Trace(1, "received an sms.");
        Trace(1, "sms sim card storage info, used:%d, total:%d", storageInfo.used, storageInfo.total);
        handle_sms(event);
        
        break;
    }
    case API_EVENT_ID_NETWORK_REGISTER_DENIED:
    case API_EVENT_ID_NETWORK_REGISTER_NO:
    case API_EVENT_ID_NETWORK_REGISTERED_HOME:
    case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
    case API_EVENT_ID_NETWORK_DETACHED:
    case API_EVENT_ID_NETWORK_ATTACH_FAILED:
    case API_EVENT_ID_NETWORK_ATTACHED:
    case API_EVENT_ID_NETWORK_DEACTIVED:
    case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
    case API_EVENT_ID_NETWORK_ACTIVATED:
    case API_EVENT_ID_SIGNAL_QUALITY:
    case API_EVENT_ID_NETWORK_GOT_TIME:
    case API_EVENT_ID_NETWORK_CELL_INFO:
        network_event_dispatch(event);
        break;
    default:
        break;
    }
}