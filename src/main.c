#include <stdio.h>

#include "blue2th.h"


#define STANDARD_INQUIRY_SEC 10.24


int main()
{
    b2th_device_t *local_device = b2th_local_device_get_first();
    if (!local_device)
        return -1;

    printf("First local bluetooth controller:[%s][%s]\n", local_device->name, local_device->address);

    b2th_device_t *remote_device = b2th_device_scan(local_device, STANDARD_INQUIRY_SEC);
    if (!remote_device) {
        b2th_device_deinit(local_device);
        return -1;
    }

    printf("%ld bluetooth device has been found.\n", b2th_device_size(remote_device));

    b2th_device_t *pos;
    b2th_device_for_each_entry(remote_device, pos)
        printf("[%s][%s]\n", pos->address, pos->name);

    const char *bt_iface_test = "SelDeGuérandeAOC";
    b2th_device_t *bt = b2th_get_device_by_name(remote_device, bt_iface_test);
    if (!bt) {
        printf("Bluetooth iface [%s] not found.\n", bt_iface_test);
        b2th_device_deinit(local_device);
        b2th_device_deinit(remote_device);
        return -1;
    }

    printf("\nBluetooth iface %s found on %s.\n\n", bt->name, bt->address);

    b2th_device_deinit(local_device);
    b2th_device_deinit(remote_device);

    return 0;
}

