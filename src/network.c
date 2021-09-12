#include "includes/network.h"
#include "includes/sms.h"

static bool attach_activate()
{
    uint8_t status;
    bool ret = Network_GetAttachStatus(&status);
    if (!ret)
    {
        Trace(2, "get attach status failed");
        return false;
    }
    Trace(2, "attach status: %d", status);
    if (!status)
    {
        Trace(2, "trying to attach");
        ret = Network_StartAttach();
        if (!ret)
        {
            Trace(2, "network attach failed");
            return false;
        }
    }
    else
    {
        ret = Network_GetActiveStatus(&status);
        if (!ret)
        {
            Trace(2, "get activate status failed");
            return false;
        }
        Trace(2, "activate status:%d", status);
        if (!status)
        {
            Trace(2, "trying to activate");
            Network_PDP_Context_t context = {
                .apn = PDP_CONTEXT_APN,
                .userName = PDP_CONTEXT_USERNAME,
                .userPasswd = PDP_CONTEXT_PASSWD};
            Network_StartActive(context);
        }
    }
    return true;
}

void network_event_dispatch(API_Event_t *pEvent)
{
    switch (pEvent->id)
    {
    case API_EVENT_ID_NETWORK_REGISTER_DENIED:
        Trace(2, "network register denied");
        gprs_registration = false;
        break;

    case API_EVENT_ID_NETWORK_REGISTER_NO:
        Trace(2, "network register rejected");
        gprs_registration = false;
        break;

    case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        Trace(2, "network register success [HOME]");
        gprs_registration = true;
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
        Trace(2, "network register success [ROAMING]");
        gprs_registration = true;
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_DETACHED:
        Trace(2, "network detached");
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_ATTACH_FAILED:
        Trace(2, "network attach failed");
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_ATTACHED:
        Trace(2, "network attach success");
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_DEACTIVED:
        Trace(2, "network deactived");
        attach_activate();
        gprs_registration = false;
        break;

    case API_EVENT_ID_NETWORK_ACTIVATE_FAILED:
        Trace(2, "network activate failed");
        attach_activate();
        break;

    case API_EVENT_ID_NETWORK_ACTIVATED:
        Trace(2, "network activate success");
        gprs_registration = true;
        delete_sms_messages();
        break;

    case API_EVENT_ID_SIGNAL_QUALITY:
        Trace(2, "signal quality: %d", pEvent->param1);
        break;

    case API_EVENT_ID_NETWORK_GOT_TIME: //pParam1: RTC_Time_t*
        Trace(2, "network got time");
        break;

    case API_EVENT_ID_NETWORK_CELL_INFO: //param1:cell number(1 serving cell and param1-1 neighbor cell) , pParam1: Network_Location_t*
        Trace(2, "network got cell info");
        break;

    default:
        break;
    }
}
