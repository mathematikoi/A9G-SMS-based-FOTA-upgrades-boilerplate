#include "includes/sms.h"
#include "includes/fota.h"

void delete_sms_messages(void)
{
    Trace(1, "deleting sms messages from sim card");
    for (int i = 1; i <= 10; i++)
    {
        if (!SMS_DeleteMessage(i, SMS_STATUS_ALL, SMS_STORAGE_SIM_CARD))
            Trace(1, "deleting sms failed");
    }
    Trace(1, "sms messages deleted");
}

void handle_sms(API_Event_t *event)
{
    uint32_t content_length = event->param2;
    if (!content_length)
        return;
    uint8_t *header = event->pParam1;
    uint8_t *content = event->pParam2;
    uint8_t sms_type = content[0] - 32;
    Trace(1, "sms headerrrrrrrrrrr: %s", header);
    Trace(1, "sms content: %s", content);
    Trace(1, "sms type: %d", sms_type);
    switch (sms_type)
    {
    case SMS_FOTA:
        handle_fota_packet(content + 1, content_length - 1);
        break;

    default:
        break;
    }
    delete_sms_messages();
}

void sms_init(void)
{

    if (!SMS_SetNewMessageStorage(SMS_STORAGE_SIM_CARD))
    {
        Trace(1, "sms set message storage fail");
        return;
    }
}