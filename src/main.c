#include <stdio.h>

#include "blue2th.h"


#define STANDARD_INQUIRY_SEC 10.24


int main()
{
    // Get first local device
    b2th_device_t *local_device = b2th_local_device_get_first();
    if (!local_device)
        goto clean;

    printf("First local bluetooth controller:[%s][%s]\n", local_device->name, local_device->address);

    // Get a list of local device
    b2th_list_t *local_device_l = b2th_local_device_get_list();
    if (!local_device_l)
        goto clean_local_device;

    // Print all local devices available
    b2th_device_t *pos;
    b2th_device_for_each_entry(local_device_l, pos)
        printf("Device list -[%s][%s]\n", pos->address, pos->name);

    // Scan on the first local device
    b2th_list_t *remote_device = b2th_device_scan(local_device, STANDARD_INQUIRY_SEC);
    if (!remote_device)
        goto clean_local_device_list;

    // Print number of remote devices
    printf("%ld bluetooth device has been found.\n", b2th_list_size(remote_device));

    // Print all remote devices found
    b2th_device_for_each_entry(remote_device, pos)
        printf("[%s][%s]\n", pos->address, pos->name);

    // Check on specific bluetooth interface
    const char *bt_iface_test = "SelDeGuérandeAOC";
    b2th_device_t *bt = b2th_get_device_by_name(remote_device, bt_iface_test);
    if (!bt) {
        printf("Bluetooth iface [%s] not found.\n", bt_iface_test);
        goto clean_remote_device;
    }

    printf("\nBluetooth iface %s found on %s.\n\n", bt->name, bt->address);

/*
    // Connect with pairing to specified bluetooth interface
    if (b2th_device_pairing(bt) == -1) {
        printf("Unable to connect to bluetooth iface [%s].\n", bt->name);
        goto clean_remote_device;
        return -1;
    }
*/

    // Free objects
    b2th_list_deinit(remote_device);
    b2th_list_deinit(local_device_l);
    b2th_device_deinit(local_device);

    return 0;


clean_remote_device:
    b2th_list_deinit(remote_device);
clean_local_device_list:
    b2th_list_deinit(local_device_l);
clean_local_device:
    b2th_device_deinit(local_device);
clean:
    return -1;
}

