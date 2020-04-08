#include <stdio.h>

#include "blue2th.h"


#define STANDARD_INQUIRY_SEC 10.24


int main()
{
    b2th_t *bt = b2th_init(NULL);
    if (!bt)
        return -1;

    if (b2th_scan(bt, STANDARD_INQUIRY_SEC) == -1) {
        b2th_deinit(bt);
        return -1;
    }

    printf("%d bluetooth device has been found.\n", bt->nb_device);

    b2th_node_t *pos;
    b2th_list_for_each_entry(bt, pos)
        printf("[%s][%s]\n", pos->addr, pos->name);

    b2th_deinit(bt);

    return 0;
}

