#include "list.h"

list_node_t *list_node_new(void *val)
{
  list_node_t *self;

  if((self = LIST_MALLOC(sizeof(list_node_t))) == NULL)
  {
    return NULL;
  }

  self->val = val;
  self->prev = NULL;
  self->next = NULL;
  
  return self;
}
