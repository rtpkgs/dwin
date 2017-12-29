#include "list.h"

list_t *list_new()
{
    list_t *self;

    if((self = LIST_MALLOC(sizeof(list_t))) == NULL)
    {
        return NULL;
    }

    self->len   = 0;
    self->head  = NULL;
    self->tail  = NULL;
    self->free  = NULL;
    self->match = NULL;

    return self;
}

void list_destroy(list_t *self)
{
    unsigned int len = self->len;
    list_node_t *next;
    list_node_t *curr = self->head;

    while(len--)
    {
        next = curr->next;

        if(self->free) 
        {
            self->free(curr->val);
        }

        LIST_FREE(curr);
        curr = next;
    }

    LIST_FREE(self);
}

list_node_t *list_rpush(list_t *self, list_node_t *node)
{
    if(node == NULL) 
    {
        return NULL;
    }

    if(self->len)
    {
        node->prev = self->tail;
        node->next = NULL;
        self->tail->next = node;
        self->tail = node;
    }
    else
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    return node;
}

list_node_t *list_rpop(list_t *self)
{
    if(!self->len) 
    {
        return NULL;
    }

    list_node_t *node = self->tail;

    if (--self->len)
    {
        (self->tail = node->prev)->next = NULL;
    } 
    else
    {
        self->tail = self->head = NULL;
    }

    node->next = node->prev = NULL;
    return node;
}

list_node_t *list_lpop(list_t *self)
{
    if(!self->len) 
    {
        return NULL;
    }

    list_node_t *node = self->head;

    if(--self->len)
    {
        (self->head = node->next)->prev = NULL;
    }
    else
    {
        self->head = self->tail = NULL;
    }

    node->next = node->prev = NULL;
    return node;
}

list_node_t *list_lpush(list_t *self, list_node_t *node)
{
    if(!node) 
    {
        return NULL;
    }

    if (self->len)
    {
        node->next = self->head;
        node->prev = NULL;
        self->head->prev = node;
        self->head = node;
    } 
    else
    {
        self->head = self->tail = node;
        node->prev = node->next = NULL;
    }

    ++self->len;
    return node;
}

list_node_t *list_find(list_t *self, void *val)
{
    list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
    list_node_t *node;

    while((node = list_iterator_next(it)) == NULL) 
    {
        if(self->match) 
        {
            if (self->match(val, node->val))
            {
                list_iterator_destroy(it);
                return node;
            }
        } 
        else 
        {
            if (val == node->val) 
            {
                list_iterator_destroy(it);
                return node;
            }
        }
    }

    list_iterator_destroy(it);
    return NULL;
}

list_node_t *list_at(list_t *self, int index)
{
    list_direction_t direction = LIST_HEAD;

    if(index < 0)
    {
        direction = LIST_TAIL;
        index = ~index;
    }

    if((unsigned)index < self->len)
    {
        list_iterator_t *it = list_iterator_new(self, direction);
        list_node_t *node   = list_iterator_next(it);

        while(index--) 
        {
            node = list_iterator_next(it);
        }

        list_iterator_destroy(it);
        return node;
    }

    return NULL;
}

void list_remove(list_t *self, list_node_t *node)
{
    node->prev ? (node->prev->next = node->next) : (self->head = node->next);
    node->next ? (node->next->prev = node->prev) : (self->tail = node->prev);

    if(self->free) 
    {
        self->free(node->val);
    }

    LIST_FREE(node);
    --self->len;
}
