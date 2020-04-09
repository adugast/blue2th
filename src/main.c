#include <stdio.h>

#include "blue2th.h"


#define STANDARD_INQUIRY_SEC 10.24


int main()
{
    b2th_device_t *local_device = b2th_local_device_get_list();
    if (!local_device)
        return -1;

    b2th_device_t *pos;
    b2th_device_for_each_entry(local_device, pos)
        printf("Local controller:[%s][%s]\n", pos->name, pos->address);

    b2th_device_t *remote_device = b2th_device_scan(local_device, STANDARD_INQUIRY_SEC);
   if (!remote_device) {
        b2th_device_deinit(local_device);
        return -1;
    }

    printf("%ld bluetooth device has been found.\n", b2th_device_size(remote_device));

    b2th_device_for_each_entry(remote_device, pos)
        printf("[%s][%s]\n", pos->address, pos->name);

    b2th_device_deinit(local_device);
    b2th_device_deinit(remote_device);

    return 0;
}

