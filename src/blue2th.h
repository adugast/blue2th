#ifndef __BLUE2TH_H__
#define __BLUE2TH_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "list.h"


typedef struct {
    char *addr;             /*<! remote bluetooth interface form:"XX:XX:XX:XX:XX:XX" */
    char *name;             /*<! remote bluetooth name */
    list_t node;            /*<! linked list node */
} b2th_node_t;              /*<! typedef b2th_node_t */


typedef struct {
    char *interface;        /*<! local bluetooth interface form:"XX:XX:XX:XX:XX:XX" */
    unsigned int nb_device; /*<! nb of devices found */
    list_t list_head;       /*<! head of b2th_node_t */
} b2th_t;                   /*<! typedef b2th_t */


/*!
 * \brief b2th_list_for_each_entry - iterate over a b2th device list
 *
 * \param[in]   pos     the b2th_node_t ptr used as a loop counter to access fields.
 * \param[in]   head    the b2th_t ptr that contains the head of the list.
 */
#define b2th_list_for_each_entry(head, pos) \
    list_for_each_entry(pos, &(head->list_head), node)


/*!
 * \brief b2th_list_for_each_entry_safe - iterate safely over a b2th device list (to free nodes)
 *
 * \param[in]   pos     the b2th_node_t ptr used as a loop counter to access fields.
 * \param[in]   save    the b2th_node_t save ptr used as temporary storage.
 * \param[in]   head    the b2th_t ptr that contains the head of the list.
 */
#define b2th_list_for_each_entry_safe(head, pos, save) \
    list_for_each_entry_safe(pos, save, &(head->list_head), node)


/*!
 * \brief b2th_init - Create a b2th handler
 *
 * \param[in]   local_interface     address of the bluetooth interface. \
 * form: "XX:XX:XX:XX:XX:XX"
 * passing NULL takes first available bluetooth interface.
 *
 * \return  b2th handler on success, NULL on error.
 */
b2th_t *b2th_init(const char *local_interface);


/*!
 * \brief b2th_scan - Launch a scan and fill b2th handler with results
 *
 * \param[in]   bt      b2th handler.
 * \param[in]   secs    time in seconds that the bluetooth scan runs.
 *
 * \return  0 on success, -1 on error.
 */
int b2th_scan(b2th_t *bt, unsigned int secs);


/*!
 * \brief b2th_deinit - Free a b2th handler
 *
 * \param[in]   bt     bt handler to free.
 */
void b2th_deinit(b2th_t *bt);


/*!
 * \brief b2th_get_device_by_name - Get b2th_node_t thanks to the bluetooth interface name
 *
 * \param[in]   bt      b2th handler.
 * \param[in]   name    name of the b2th_node to retreive.
 *
 * \return  bl2th_node_t on success, NULL on error.
 */
b2th_node_t *b2th_get_device_by_name(b2th_t *bt, const char *name);


/*!
 * \brief b2th_get_device_by_addr - Get b2th_node_t thanks to the bluetooth interface address
 *
 * \param[in]   bt      b2th handler.
 * \param[in]   address address of the b2th_node to retreive.
 *
 * \return  bl2th_node_t on success, NULL on error.
 */
b2th_node_t *b2th_get_device_by_addr(b2th_t *bt, const char *addr);


#ifdef __cplusplus
}
#endif


#endif /* __BLUE2TH_H__ */

