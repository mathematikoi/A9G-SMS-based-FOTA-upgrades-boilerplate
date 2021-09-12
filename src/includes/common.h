#ifndef COMMON_H
#define COMMON_H
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include <api_event.h>
#include <api_debug.h>
#include "api_fota.h"
#include <api_socket.h>
#include <api_network.h>
#include "api_fs.h"
#include <api_os.h>
#include <api_gps.h>
#include <api_sms.h>
#include <api_hal_uart.h>
#include "time.h"

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);

#endif