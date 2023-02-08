#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long ulong;

typedef struct node
{
   struct node *previous;
   struct node *next;
   void *item;
} NODE;

typedef struct list
{
   long size;
   NODE *first;
   NODE *last;
} LIST;

void add_item(LIST *list, void *item);
void remove_item(LIST *list, void *item);
ulong index_of(LIST *list, void *item);
void *item_at(LIST *list, ulong index);

#endif