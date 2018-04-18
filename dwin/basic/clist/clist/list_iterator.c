#include "list.h"

list_iterator_t *list_iterator_new(list_t *list, list_direction_t direction)
{
    list_node_t *node = direction == LIST_HEAD ? list->head : list->tail;

    return list_iterator_new_from_node(node, direction);
}

list_iterator_t *list_iterator_new_from_node(list_node_t *node, list_direction_t direction)
{
    list_iterator_t *self;

    if((self = LIST_MALLOC(sizeof(list_iterator_t))) == NULL)
    {
        return NULL;
    }

    self->next = node;
    self->direction = direction;

    return self;
}

list_node_t *list_iterator_next(list_iterator_t *self)
{
    list_node_t *curr = self->next;
    if(curr)
    {
        self->next = self->direction == LIST_HEAD ? curr->next : curr->prev;
    }
    return curr;
}

void list_iterator_destroy(list_iterator_t *self)
{
    LIST_FREE(self);
    self = NULL;
}
