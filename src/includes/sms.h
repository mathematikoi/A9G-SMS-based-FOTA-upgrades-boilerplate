#ifndef SMS_H
#define SMS_H

#include "common.h"

#define SMS_FOTA 0
#define SMS_TOKEN 1

void handle_sms(API_Event_t *event);
void sms_init(void);
void delete_sms_messages(void);

#endif