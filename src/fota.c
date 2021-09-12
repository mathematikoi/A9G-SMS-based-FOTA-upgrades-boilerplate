#include "includes/fota.h"
#include "includes/fs.h"

static void process_fota_upgrade()
{
    uint64_t fota_package_base64_size = fs_file_size(FOTA_FILE);
    if (fota_package_base64_size <= 0)
        goto upgrade_failed;

    uint8_t *fota_package_base64 = (uint8_t *)OS_Malloc(fota_package_base64_size);
    if (!fota_package_base64)
    {
        Trace(1, "error allocating memory for fota upgrade package");
        return;
    }

    bool readdin = fs_read(FOTA_FILE, fota_package_base64, fota_package_base64_size);
    if (!readdin)
    {
        Trace(1, "error reading upgrade package from fota file");
        return;
    }
    uint32_t sum = 0;
    for (uint32_t i = 0; i < fota_package_base64_size; i++)
    {
        sum += fota_package_base64[i];
    }
    Trace(1, "read fota_package_base64: %s", fota_package_base64);
    Trace(1, "read fota_package_base64_size: %d", fota_package_base64_size);
    Trace(1, "fota_package_base64: %d", sum);

    uint32_t fota_package_size;
    uint8_t *fota_package = base64_decode(fota_package_base64, fota_package_base64_size, &fota_package_size);

    if (!fota_package)
    {
        Trace(1, "error decoding package upgrade");
        goto upgrade_failed;
    }

    Trace(1, "decoded the package upgrade, length: %d", fota_package_size);

    if (!API_FotaInit(fota_package_size) || API_FotaReceiveData(fota_package, (int)fota_package_size) == 0)
    {
        Trace(1, "API_FotaReceiveData: error!");
        goto upgrade_failed;
    }

    return;

upgrade_failed:
    Trace(1, "fota failed!");
    API_FotaClean();
    OS_Free(fota_package_base64);
    OS_Free(fota_package);
    API_FS_Delete(FOTA_FILE);
    //TODO: delete sms messages after upgrade is done
}

void handle_fota_packet(uint8_t *data, uint16_t size)
{
    // dismiss packet as it has no upgrade data.
    if (size <= 2)
        return;

    uint8_t packet_order = data[0] - 32;
    uint8_t packets_count = data[1] - 32;

    uint8_t *packet = data + 2;
    uint8_t packet_size = size - 2;

    Trace(1, "fota packet order: %d", packet_order);
    Trace(1, "fota packets count: %d", packets_count);

    // check if it's the first packet
    if (packet_order == 1)
    {

        Trace(1, "this is the first fota packet");
        API_FS_Delete(FOTA_FILE);
        fs_write(FOTA_FILE, packet, packet_size, FS_O_CREAT | FS_O_WRONLY | FS_O_TRUNC, 0);
    }
    else
    {
        int64_t fota_file_size = fs_file_size(FOTA_FILE);
        if (fota_file_size < 0)
            return;
        uint8_t received_fota_packets_count = fota_file_size / FOTA_PACKET_SIZE + !!(fota_file_size % FOTA_PACKET_SIZE);

        Trace(1, "received fota packets count %d", received_fota_packets_count);

        // checking if packet is in the right order
        if (packet_order - received_fota_packets_count != 1)
        {
            Trace(1, "packet is out of order, dismissed");
            return;
        }

        bool written = fs_write(FOTA_FILE, packet, packet_size, FS_O_WRONLY | FS_O_APPEND, 0);
        if (!written)
        {
            return;
        }
        // check if it's the last packet
        if (packets_count - received_fota_packets_count == 1)
        {
            Trace(1, "this is the last packet, proceeding with the upgrade");
            process_fota_upgrade();
        }
    }
}