#include "includes/fs.h"

int32_t fs_open(uint8_t *path, uint32_t operationFlag, uint32_t mode)
{
    int32_t fd;

    fd = API_FS_Open(path, operationFlag, mode);
    if (fd < 0)
    {
        // TODO: persist error for logging
        Trace(1, "error opening  file:%d", fd);
    }
    return fd;
}

int64_t fs_file_size(uint8_t *path)
{
    int64_t size = API_FS_GetFileSize(fs_open(path, FS_O_RDONLY, 0));
    if (size < 0)
    {
        // TODO: persist error for logging
        Trace(1, "error getting file size");
    }
    return size;
}

bool fs_write(uint8_t *path, void *data, uint32_t size, uint32_t operationFlag, uint32_t mode)
{
    int32_t fd;
    int32_t ret;

    fd = fs_open(path, operationFlag, mode);
    if (fd < 0)
        return false;

    ret = API_FS_Write(fd, (uint8_t *)data, size);
    API_FS_Close(fd);
    if (ret <= 0)
    {
        // TODO: persist error for logging
        Trace(1, "error writing to  file:%d", fd);
        return false;
    }
    return true;
}

bool fs_read(uint8_t *path, void *data, uint32_t size)
{
    int32_t fd;
    uint32_t readdin;

    fd = fs_open(path, FS_O_RDONLY, 0);
    if (fd < 0)
        return false;

    readdin = API_FS_Read(fd, data, size);
    API_FS_Close(fd);
    if (readdin != size)
    {
        // TODO: persist error for logging
        Trace(1, "error reading   file:%s", path);
        return false;
    }
    return true;
}