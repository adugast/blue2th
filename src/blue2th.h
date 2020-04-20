#ifndef __BLUE2TH_H__
#define __BLUE2TH_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "list.h"


/*!
 * \file blue2th.h
 *
 * \brief blue2th api definition
 */


/*!
 * \brief blue2th device object
 */
typedef struct {
    char *address;          /**<! bluetooth 48-bit device address */
    char *name;             /**<! bluetooth user friendly string name */
    list_t node;            /**<! linked list node */
} b2th_device_t;


/*!
 * \brief b2th_device_for_each_entry - iterate over a b2th device list
 *
 * \param[in]   head    the b2th_device_t ptr that contains the head of the list.
 * \param[in]   pos     the b2th_device_t ptr used as a loop counter to access fields.
 */
#define b2th_device_for_each_entry(head, pos) \
    list_for_each_entry(pos, &(head->node), node)


/*!
 * \brief b2th_device_for_each_entry_safe - iterate safely over a b2th device list (keep safe ptr to free nodes)
 *
 * \param[in]   head    the b2th_device_t ptr that contains the head of the list.
 * \param[in]   pos     the b2th_device_t ptr used as a loop counter to access fields.
 * \param[in]   save    the b2th_device_t save ptr used as temporary storage.
 */
#define b2th_device_for_each_entry_safe(head, pos, save) \
    list_for_each_entry_safe(pos, save, &(head->node), node)


/*!
 * \brief b2th_local_device_get_fist - Get first available bluetooth interface
 *
 * \return  first available b2th interface on success, NULL on error.
 */
b2th_device_t *b2th_local_device_get_first();


/*!
 * \brief b2th_local_device_get_list - Get list of available bluetooth interface
 *
 * \return  list of available b2th interface on success, NULL on error.
 */
b2th_device_t *b2th_local_device_get_list();


/*!
 * \brief b2th_device_deinit - Free a b2th device handler
 *
 * \param[in]   bd     the b2th device handler to free.
 */
void b2th_device_deinit(b2th_device_t *bd);


/*!
 * \brief b2th_device_scan - Launch a scan and return the list of b2th device found
 *
 * \param[in]   local_device   local b2th device handler.
 * \param[in]   secs           time in seconds that the bluetooth scan runs.
 *
 * \return  b2th_device_t list on success, NULL on error.
 */
b2th_device_t *b2th_device_scan(b2th_device_t *local_device, unsigned int secs);


/*!
 * \brief b2th_get_device_by_name - Get a b2th device thanks to its bluetooth interface name
 *
 * \param[in]   bd      b2th device handler.
 * \param[in]   name    b2th device name to retrieve.
 *
 * \return  b2th_device_t on success, NULL on error.
 */
b2th_device_t *b2th_get_device_by_name(b2th_device_t *bd, const char *name);


/*!
 * \brief b2th_get_device_by_addr - Get a b2th device thanks to its bluetooth interface address
 *
 * \param[in]   bd      b2th device handler.
 * \param[in]   address b2th device address to retrieve.
 *
 * \return  b2th_device_t on success, NULL on error.
 */
b2th_device_t *b2th_get_device_by_addr(b2th_device_t *bd, const char *address);


/*!
 * \brief b2th_device_size - Get b2th device list size
 *
 * \param[in]   bd      b2th device handler.
 *
 * \return  b2th list size on success.
 */
size_t b2th_device_size(b2th_device_t *bd);


#ifdef __cplusplus
}
#endif


#endif /* __BLUE2TH_H__ */

