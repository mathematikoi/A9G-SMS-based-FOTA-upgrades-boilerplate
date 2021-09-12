#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

#define PDP_CONTEXT_APN "hologram"
#define PDP_CONTEXT_USERNAME ""
#define PDP_CONTEXT_PASSWD ""

extern bool gprs_registration;

void network_event_dispatch(API_Event_t *pEvent);

#endif