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


static struct hci_dev_list_req *__get_bluetooth_device_list(int bluetooth_fd)
{
    // Allocate memory for the devices list + the maximum HCI_MAX_DEV devices
    struct hci_dev_list_req *hdlr = calloc(1, sizeof(struct hci_dev_list_req)
            + HCI_MAX_DEV * sizeof(struct hci_dev_req));
    if (!hdlr) {
        perror("Failed allocate HCI device request memory");
        return NULL;
    }

    // Fill HCI_MAX_DEV in dev_num to prepare the ioctl request
    hdlr->dev_num = HCI_MAX_DEV;

    // Retrieve every bluetooth controller available
    if (ioctl(bluetooth_fd, HCIGETDEVLIST, hdlr) == -1) {
        perror("Failed to get HCI device list");
        free(hdlr);
        return NULL;
    }

    return hdlr;
}


static int b2th_device_add_node(b2th_device_t *bd, const char *name, const char *address)
{
    b2th_device_t *bd_new = calloc(1, sizeof(b2th_device_t));
    if (!bd)
        return -1;

    bd_new->address = strdup(address);
    bd_new->name = strdup(name);

    list_add_tail(&(bd_new->node), &(bd->node));

    return 0;
}


b2th_device_t *b2th_device_init()
{
    b2th_device_t *bd = calloc(1, sizeof(b2th_device_t));
    if (!bd)
        return NULL;

    bd->address = NULL;
    bd->name = NULL;

    init_list(&(bd->node));

    return bd;
}


void b2th_device_deinit(b2th_device_t *bd)
{
    b2th_device_t *pos, *save;
    b2th_device_for_each_entry_safe(bd, pos, save) {
        list_del(&(pos->node));
        free(pos->address);
        free(pos->name);
        free(pos);
    }

    free(bd);
}


enum b2th_field_e {
    FIRST,
    LIST
};


static b2th_device_t *__b2th_get_device(enum b2th_field_e field)
{
    b2th_device_t *bd = b2th_device_init();
    if (!bd)
        return NULL;

    int bluetooth_fd = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (bluetooth_fd == -1) {
        perror("Failed to open raw HCI socket");
        free(bd);
        return NULL;
    }

    struct hci_dev_list_req *hdlr = __get_bluetooth_device_list(bluetooth_fd);
    if (!hdlr) {
        free(bd);
        close(bluetooth_fd);
        return NULL;
    }

    int i;
    for (i = 0; i < hdlr->dev_num; i++) {

        struct hci_dev_info di = {
            .dev_id = (hdlr->dev_req + i)->dev_id
        };

        if (ioctl(bluetooth_fd, HCIGETDEVINFO, &di) == -1)
            continue;

        char addr[18];
        ba2str(&di.bdaddr, addr);

        b2th_device_add_node(bd, di.name, addr);
        if (field == FIRST)
            break;
    }

    close(bluetooth_fd);
    free(hdlr);

    return bd;
}


b2th_device_t *b2th_local_device_get_first()
{
    return __b2th_get_device(FIRST);
}


b2th_device_t *b2th_local_device_get_list()
{
    return __b2th_get_device(LIST);
}


struct b2th_inquiry {
    int dev_id;
    int max_rsp;
    int secs;
    long flags;
};


static int b2th_scan_device_id(b2th_device_t *remote_device, struct b2th_inquiry *bi)
{
    inquiry_info *ii = NULL;
    int num_rsp = hci_inquiry(bi->dev_id, bi->secs, bi->max_rsp, NULL, &ii, bi->flags);
    if (num_rsp < 0) {
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

        b2th_device_add_node(remote_device, addr, name);
    }

    hci_close_dev(sock);

    free(ii);

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


b2th_device_t *b2th_device_scan(b2th_device_t *local_device, unsigned int secs)
{
    if (local_device == NULL) {
        printf("Bluetooth object not initialized\n");
        return NULL;
    }

    int dev_id = b2th_get_dev_id(local_device->address);
    if (dev_id < 0) {
        printf("Couldn't retrieve bluetooth interface\n");
        return NULL;
    }

    struct b2th_inquiry bi = {
        .dev_id = dev_id,
        .max_rsp = 255,
        .secs = secs,
        .flags = IREQ_CACHE_FLUSH,
    };

    b2th_device_t *remote_device = b2th_device_init();
    if (!remote_device)
        return NULL;

    b2th_scan_device_id(remote_device, &bi);

    return remote_device;
}


b2th_device_t *b2th_get_device_by_name(b2th_device_t *bd, const char *name)
{
    b2th_device_t *pos;
    b2th_device_for_each_entry(bd, pos)
        if (strncmp(pos->name, name, strlen(pos->name)) == 0)
            return pos;

    return NULL;
}


b2th_device_t *b2th_get_device_by_addr(b2th_device_t *bd, const char *addr)
{
    b2th_device_t *pos;
    b2th_device_for_each_entry(bd, pos)
        if (strncmp(pos->address, addr, strlen(pos->address)) == 0)
            return pos;

    return NULL;
}


size_t b2th_device_size(b2th_device_t *bd)
{
    return list_length(&(bd->node));
}


/*
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
*/

