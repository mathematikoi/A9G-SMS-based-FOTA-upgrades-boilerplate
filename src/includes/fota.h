#ifndef FOTA_H
#define FOTA_H

#include "common.h"

#define FOTA_FILE "/fota"
#define FOTA_PACKET_SIZE 156

void handle_fota_packet(uint8_t *data, uint16_t size);

#endif