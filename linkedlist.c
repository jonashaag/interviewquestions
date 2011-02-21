#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef char data_t;
#define PRINTF_DATA_SPECIFIER "%c"

typedef struct linkedlist_ {
  data_t data;
  struct linkedlist_* next;
} linkedlist;

linkedlist*
linkedlist_new(data_t data)
{
  linkedlist* list;
  list = malloc(sizeof(linkedlist));
  list->next = NULL;
  list->data = data;
  return list;
}

void linkedlist_free(linkedlist* list)
{
  linkedlist* tmp;
  while(list) {
    tmp = list;
    list = list->next;
    free(tmp);
  }
}

linkedlist*
linkedlist_prepend(linkedlist* list, data_t data)
{
  linkedlist* new_list;
  new_list = linkedlist_new(data);
  new_list->next = list;
  return new_list;
}

int
linkedlist_find_with_prev(linkedlist* list, data_t data,
                          linkedlist** node, linkedlist** prevnode)
{
  linkedlist* prev = NULL;
  while(list) {
    if(list->data == data) {
      *node = list;
      *prevnode = prev;
      return 1;
    }
    prev = list;
    list = list->next;
  }
  return 0;
}

int linkedlist_find(linkedlist* list, data_t data, linkedlist** node)
{
  while(list) {
    if(list->data == data) {
      *node = list;
      return 1;
    }
    list = list->next;
  }
  return 0;
}

int
linkedlist_delete(linkedlist* list, data_t data, linkedlist** newlist)
{
  linkedlist *node, *prevnode;
  if(!linkedlist_find_with_prev(list, data, &node, &prevnode))
    return 0;
  if(node == list) {
    assert(prevnode == NULL);
    list = node->next;
  } else {
    prevnode->next = node->next;
  }
  free(node);
  *newlist = list;
  return 1;
}

linkedlist*
linkedlist_split(linkedlist* list, data_t data)
{
  linkedlist *middle, *result;
  if(!linkedlist_find(list, data, &middle))
    return NULL;
  result = middle->next;
  middle->next = NULL;
  return result;
}

int
linkedlist_has_cycle(linkedlist* list)
{
  linkedlist *slow_cursor, *fast_cursor;
  slow_cursor = fast_cursor = list;
  while(fast_cursor) {
    fast_cursor = fast_cursor->next;
    if(fast_cursor == slow_cursor)
      return 1;
    if(!fast_cursor) return 0;
    fast_cursor = fast_cursor->next;
    if(fast_cursor == slow_cursor)
      return 1;
    slow_cursor = slow_cursor->next;
  }
  return 0;
}

linkedlist*
linkedlist_find_middle(linkedlist* list)
{
  size_t length = 0;
  linkedlist *slow_cursor, *fast_cursor;
  slow_cursor = fast_cursor = list;
  while(fast_cursor && (fast_cursor = fast_cursor->next) &&
        (slow_cursor = slow_cursor->next)) {
    ++length;
    fast_cursor = fast_cursor->next;
  }
  return slow_cursor;
}

void
linkedlist_print(linkedlist* list)
{
  printf("(");
  while(list) {
    if(list->next) {
      printf(PRINTF_DATA_SPECIFIER " ", list->data);
      list = list->next;
    } else {
      printf(PRINTF_DATA_SPECIFIER, list->data);
      break;
    }
  }
  printf(")\n");
}

linkedlist*
linkedlist_fromargv(int argc, char** argv)
{
  linkedlist* result = NULL;
  while(argc--)
    result = linkedlist_prepend(result, argv[argc][0]);
  return result;
}

linkedlist*
linkedlist_from_data_buf(data_t* buf)
{
  linkedlist* result = NULL;
  while(*buf != 0)
    result = linkedlist_prepend(result, *buf++);
  return result;
}

int main()
{
  linkedlist* tmp;
  char buf[] = {'g', 'f', 'e', 'd', 'c', 'b', 'a', '\0'};
  linkedlist* l = linkedlist_from_data_buf(buf);
  linkedlist_print(l);
  assert(!linkedlist_has_cycle(l));
  assert(linkedlist_find_middle(l) == l->next->next->next);
  linkedlist* l2 = linkedlist_split(l, 'd');
  linkedlist_print(l);
  linkedlist_print(l2);
  assert(linkedlist_find_middle(l) == l->next->next);
  assert(linkedlist_find_middle(l2) == l2->next);
  assert((linkedlist_find(l, 'c', &tmp),tmp) == l->next->next);
  assert(!linkedlist_find(l, 'g', &tmp));
  assert(!linkedlist_has_cycle(l));
  assert(!linkedlist_has_cycle(l2));
  assert(linkedlist_find(l2, 'g', &tmp));
  assert(linkedlist_delete(l2, 'g', &l2));
  assert(!linkedlist_find(l2, 'g', &tmp));
  assert(linkedlist_delete(l2, 'e', &l2));
  linkedlist_print(l2);
  linkedlist_print(l);
  assert(!linkedlist_has_cycle(l));
  tmp = l->next->next;
  l->next->next = l;
  assert(linkedlist_has_cycle(l));
  l->next->next = tmp;
  assert(!linkedlist_has_cycle(l));
  tmp = l->next->next->next;
  l->next->next->next = l;
  assert(linkedlist_has_cycle(l));
  l->next->next->next = tmp;
  assert(!linkedlist_has_cycle(l));
  l->next->next->next->next = l;
  assert(linkedlist_has_cycle(l));
  linkedlist_print(l);
  return 0;
}
