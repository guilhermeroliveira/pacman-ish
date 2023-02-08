#include "list.h"

static NODE *node_at(LIST *list, ulong index);
static void unlink_node(LIST *list, NODE *node);

static void initialize_node(NODE *node,
                            NODE *previous,
                            NODE *next,
                            void *item)
{
   node->previous = previous;
   node->next = next;
   node->item = item;
}

void add_item(LIST *list, void *item)
{
   if (item == NULL)
      return;

   NODE *newNode = malloc(sizeof(NODE));

   if (list->size == 0)
   {
      list->first = newNode;
      list->last = newNode;

      initialize_node(newNode, NULL, NULL, item);
   }
   else
   {
      NODE *lastNode = list->last;
      initialize_node(newNode, lastNode, NULL, item);

      lastNode->next = newNode;
      list->last = newNode;
   }

   list->size++;
}

void remove_item(LIST *list, void *item)
{
   if (item == NULL)
      return;

   if (list->size == 0)
      return;

   int indexToRemove = index_of(list, item);

   if (indexToRemove == -1)
      return;

   NODE *nodeToRemove = node_at(list, indexToRemove);
   unlink_node(list, nodeToRemove);

   list->size--;
}

static NODE *node_at(LIST *list, ulong index)
{
   NODE *node = list->first;

   for (ulong i = 0; i < index && node != NULL; i++)
      node = node->next;

   return node;
}

static void unlink_node(LIST *list, NODE *node)
{
   NODE *previousNode = node->previous;
   NODE *nextNode = node->next;

   if (previousNode == NULL)
      list->first = nextNode;
   else
      previousNode->next = nextNode;

   if (nextNode == NULL)
      list->last = previousNode;
   else
      nextNode->previous = previousNode;

   free(node);
}

ulong index_of(LIST *list, void *item)
{
   ulong index = 0;
   for (NODE *node = list->first; node != NULL; node = node->next, index++)
   {
      if (node->item == item)
         return index;
   }

   return -1;
}

void *item_at(LIST *list, ulong index)
{
   NODE *node = node_at(list, index);

   if (node == NULL)
      return NULL;

   return node->item;
}