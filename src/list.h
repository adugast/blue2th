#ifndef __LIST_H__
#define __LIST_H__


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \file list.h
 *
 * \brief kernel linked list implementation
 */


/*!
 * \brief Define SIZE_TYPE
 */
typedef __SIZE_TYPE__ size_t;


/*!
 * \brief Abstract type for linked list interface
 */
typedef struct list_head {
    struct list_head *next; /**< pointer to next field */
    struct list_head *prev; /**< pointer to previous filed */
} list_t;

/*!
 * \brief LIST_HEAD_INIT - Initialize a head of list
 *
 * \param[in]   name    name of the structure
 */
#define LIST_HEAD_INIT(name) { &(name), &(name) }

/*!
 * \brief INIT_LIST - Define and initialize a head of list
 *
 * \param[in]   name    name of the structure
 */
#define LIST_HEAD(name) \
    list_t name = LIST_HEAD_INIT(name)

/*!
 * \brief INIT_LIST_HEAD - Initialize a head of list
 *
 * \param[in]  ptr    pointer on the head of the list
 */
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


static inline void init_list(list_t *head)
{
    head->prev = head;
    head->next = head;
}

/*!
 * \brief list_entry - retrieve the struct for this entry
 *
 * \param[in]   ptr     the &struct list_head pointer.
 * \param[in]   type    the type of the struct this is embedded in.
 * \param[in]   member  the name of the list_struct member within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)


/*!
 * \brief container_of - retrieve the struct that contains the giver pointer
 *
 * \param[in]   ptr     the &struct list_head pointer.
 * \param[in]   type    the type of the struct this is embedded in.
 * \param[in]   member  the name of the list_struct member within the struct.
 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/*!
 * \brief list_for_each - iterate over a list
 *
 * \param[in]   pos     the struct list_head ptr to use as a loop counter.
 * \param[in]   head    the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*!
 * \brief list_for_each_entry - iterate over list of given type
 *
 * \param[in]   pos     the ptr type of the struct to retrieve and used as a loop counter.
 * \param[in]   head    the head for your list.
 * \param[in]   member  the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                          \
    for (pos = list_entry((head)->next, typeof(*pos), member);          \
            &pos->member != (head);                                     \
            pos = list_entry(pos->member.next, typeof(*pos), member))

/*!
 * \brief list_for_each_safe - iterate over a list safe against removal of list entry
 *
 * \param[in]   pos     the &struct list_head to use as a loop counter.
 * \param[in]   n       another &struct list_head to use as temporary storage
 * \param[in]   head    the head for your list.
 */
#define list_for_each_safe(pos, n, head)                    \
    for (pos = (head)->next, n = pos->next; pos != (head);  \
            pos = n, n = pos->next)

/*!
 * \brief list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 *
 * \param[in]   pos     the type * to use as a loop counter.
 * \param[in]   n       another type * to use as temporary storage
 * \param[in]   head    the head for your list.
 * \param[in]   member  the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)                  \
    for (pos = list_entry((head)->next, typeof(*pos), member),          \
            n = list_entry(pos->member.next, typeof(*pos), member);     \
            &pos->member != (head);                                     \
            pos = n, n = list_entry(n->member.next, typeof(*n), member))

/*!
 * \brief list_for_each_prev - iterate over a list backwards
 *
 * \param[in]   pos     the &struct list_head to use as a loop counter.
 * \param[in]   head    the head for your list.
 */
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * __list_insert - ONLY FOR INTERNAL USE
 *
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 *
 * Use as base for list_add_head and list_add_tail
 */
static inline void __list_insert(list_t *entry, list_t *prev, list_t *next)
{
    next->prev = entry;
    entry->next = next;
    entry->prev = prev;
    prev->next = entry;
}

/*!
 * \brief list_add_head - add a new entry
 *
 * \param[in]   new     new entry to be added
 * \param[in]   head    list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add_head(list_t *entry, list_t *head)
{
    __list_insert(entry, head, head->next);
}

/*!
 * \brief list_add_tail - add a new entry
 *
 * \param[in]   new     new entry to be added
 * \param[in]   head    list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(list_t *entry, list_t *head)
{
    __list_insert(entry, head->prev, head);
}

/**
 * ONLY FOR INTERNAL USE - __list_del
 *
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 *
 * Use as base for list_del and list_del_init
 */
static inline void __list_del(list_t *prev, list_t *next)
{
    next->prev = prev;
    prev->next = next;
}

/*!
 * \brief list_del - deletes entry from list.
 *
 * \param[in]   entry   the element to delete from the list.
 *
 * list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(list_t *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (void *) 0;
    entry->prev = (void *) 0;
}

/*!
 * \brief   list_del_init - deletes entry from list and reinitialize it.
 *
 * \param[in]   entry   the element to delete from the list.
 */
static inline void list_del_init(list_t *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

/*!
 * \brief   list_empty - tests whether a list is empty
 *
 * \param[in]   head    the list to test.
 */
static inline int list_empty(list_t *head)
{
    return head->next == head;
}

/*!
 * \brief list_length - return the number of structure that compose the list.
 *
 * \param   head    the head of your list.
 */
static inline size_t list_length(list_t *head)
{
    size_t len = 0;
    list_t *pos;

    list_for_each(pos, head)
        len++;

    return len;
}

/*!
 * \brief list_move - delete from one list and add as another's head
 *
 * \param[in]   list    the entry to move
 * \param[in]   head    the head that will precede our entry
 */
static inline void list_move(list_t *list, list_t *head)
{
    __list_del(list->prev, list->next);
    list_add_head(list, head);
}

/*!
 * \brief list_move_tail - delete from one list and add as another's tail
 *
 * \param[in]   list    the entry to move
 * \param[in]   head    the head that will follow our entry
 */
static inline void list_move_tail(list_t *list,
        list_t *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}


/**
 * ONLY FOR INTERNAL USE - __list_splice
 *
 * Use as base for list_splice_init and list_splice
 */
static inline void __list_splice(list_t *list,
        list_t *head)
{
    list_t *first = list->next;
    list_t *last = list->prev;
    list_t *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/*!
 * \param list_splice_init - join two lists and reinitialise the emptied list.
 *
 * \param[in]   list    the new list to add.
 * \param[in]   head    the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(list_t *list, list_t *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

/**
 * \brief list_splice - join two lists
 *
 * \param[in]   list    the new list to add.
 * \param[in]   head    the place to add it in the first list.
 */
static inline void list_splice(list_t *list, list_t *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}


#ifdef __cplusplus
}
#endif


#endif /* __LIST_H__ */

