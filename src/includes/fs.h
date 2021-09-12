#ifndef FS_H
#define FS_H

#include "common.h"

int32_t fs_open(uint8_t *path, uint32_t operationFlag, uint32_t mode);
int64_t fs_file_size(uint8_t *path);
bool fs_write(uint8_t *path, void *data, uint32_t size, uint32_t operationFlag, uint32_t mode);
bool fs_read(uint8_t *path, void *data, uint32_t size);
#endif