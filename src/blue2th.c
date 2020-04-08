#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include "blue2th.h"


struct b2th_inquiry {
    int dev_id;
    int max_rsp;
    int secs;
    long flags;
};


static int b2th_add_node(b2th_t *root, const char *addr, const char *name)
{
    b2th_node_t *b = calloc(1, sizeof(b2th_node_t));
    if (!b)
        return -1;

    b->addr = strdup(addr);
    b->name = strdup(name);

    list_add_tail(&(b->node), &(root->list_head));

    return 0;
}


static int b2th_get_dev_id(const char *interface)
{
    int dev_id = -1;

    if (interface == NULL)
        dev_id = hci_get_route(NULL); // Passing NULL argument will retrieve the id of first avalaible bluetooth interface
    else
        dev_id = hci_devid(interface);

    return dev_id;
}


static int b2th_scan_device_id(b2th_t *bt, struct b2th_inquiry *bi)
{
    inquiry_info *ii = calloc(bi->max_rsp, sizeof(inquiry_info));
    if (ii == NULL)
        return -1;

    int num_rsp = hci_inquiry(bi->dev_id, bi->secs, bi->max_rsp, NULL, &ii, bi->flags);
    if (num_rsp <= 0) {
        perror("hci_inquiry");
        free(ii);
        return -1;
    }

    int sock = hci_open_dev(bi->dev_id);
    char addr[19] = { 0 };
    char name[248] = { 0 };

    int i;
    for (i = 0; i < num_rsp; i++) {

        ba2str(&(ii+i)->bdaddr, addr);

        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
            strncpy(name, "unknown", sizeof(name) - 1);

        b2th_add_node(bt, addr, name);
    }

    hci_close_dev(sock);

    free(ii);

    bt->nb_device = list_length(&(bt->list_head));

    return 0;
}


b2th_t *b2th_init(const char *interface)
{
    b2th_t *bt = calloc(1, sizeof(b2th_t));
    if (!bt)
        return NULL;

    if (interface)
        bt->interface = strdup(interface);

    init_list(&(bt->list_head));

    return bt;
}


void b2th_deinit(b2th_t *bt)
{
    if (!bt)
        return;

    b2th_node_t *pos, *save;
    b2th_list_for_each_entry_safe(bt, pos, save) {
        list_del(&(pos->node));
        free(pos->addr);
        free(pos->name);
        free(pos);
    }

    if (bt->interface)
        free(bt->interface);

    free(bt);
}


int b2th_scan(b2th_t *bt, unsigned int secs)
{
    if (bt == NULL) {
        printf("Bluetooth object not initialized\n");
        return -1;
    }

    int dev_id = b2th_get_dev_id(bt->interface);
    if (dev_id < 0) {
        printf("Couldn't retrieve bluetooth interface\n");
        return -1;
    }

    struct b2th_inquiry bi = {
        .dev_id = dev_id,
        .max_rsp = 255,
        .secs = secs,
        .flags = IREQ_CACHE_FLUSH,
    };

    b2th_scan_device_id(bt, &bi);

    return 0;
}


b2th_node_t *b2th_get_device_by_name(b2th_t *bt, const char *name)
{
    b2th_node_t *pos;
    b2th_list_for_each_entry(bt, pos)
        if (strncmp(pos->name, name, strlen(pos->name)) == 0)
            return pos;

    return NULL;
}


b2th_node_t *b2th_get_device_by_addr(b2th_t *bt, const char *addr)
{
    b2th_node_t *pos;
    b2th_list_for_each_entry(bt, pos)
        if (strncmp(pos->addr, addr, strlen(pos->addr)) == 0)
            return pos;

    return NULL;
}


int b2th_get_local_device_addr(int dev_id)
{
    int sock = socket(AF_BLUETOOTH, SOCK_RAW|SOCK_CLOEXEC, BTPROTO_HCI);
    if (sock == -1) {
        perror("socket()");
        return -1;
    }

    struct hci_dev_info di = { .dev_id = dev_id };
    char addr[18];

    if (ioctl(sock, HCIGETDEVINFO, (void *) &di))
        return 0;

    ba2str(&di.bdaddr, addr);

    printf("\t%s\t%s\n", di.name, addr);

    return 0;
}


int b2th_write()
{
    struct sockaddr_rc addr = { 0 };
    int s, status, len=0;
    char dest[18] = "00:12:01:31:01:13";
    char buf[256];

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    if(status){
        printf(" failed to connect the device!\n");
        return -1;
    }


    do{
        len = read(s, buf, sizeof buf);

        if( len>0 ) {
            buf[len]=0;
            printf("%s\n",buf);
            write(s, buf, strlen(buf));
        }
    }while(len>0);

    close(s);
    return 0;
}
