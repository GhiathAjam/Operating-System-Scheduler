#pragma once
#include <malloc.h>
struct node
{
    void *data;
    struct node *next;
};

struct node *new_node(void *data)
{
    struct node *n = malloc(sizeof(struct node));
    n->data = data;
    n->next = NULL;
}

struct linked_list
{
    struct node *head;
    struct node *tail;
    int count;
};

void free_node(struct node *fnode)
{
    if (!fnode)
        return;
    if (fnode->next)
        free_node(fnode->next);
    free(fnode);
}
struct linked_list *new_linked_list()
{
    struct linked_list *ll = malloc(sizeof(struct linked_list));
    ll->count = 0;
    ll->head = NULL;
    ll->tail = NULL;
}
void free_linked_list(struct linked_list *llist)
{
    if (!llist)
        return;
    free_node(llist->head);
    free(llist);
}

void linked_list_push_front(struct linked_list *llist, struct node *newnode)
{

    newnode->next = llist->head;
    llist->head = newnode;
    llist->count++;
    if (!llist->tail)
        llist->tail = newnode;
}

void linked_list_push_back(struct linked_list *llist, struct node *newnode)
{

    if (!llist->head)
    {
        llist->head = newnode;
        llist->tail = newnode;
        llist->count++;
        return;
    }
    llist->tail->next = newnode;
    llist->tail = newnode;
    newnode->next = NULL;
    llist->count++;
}

void linked_list_pop_front(struct linked_list *llist, struct node **resnode)
{
    if (!llist->head){
        *resnode=NULL;
        return;
    }
    *resnode = llist->head;
    llist->head = llist->head->next;
    (*resnode)->next = NULL;
    llist->count--;
    if (llist->count == 0)
        llist->tail = NULL;
}

void linked_list_remove(struct linked_list *llist, void *data)
{
    if (!llist || !data)
        return;

    if (llist->count == 1)
    {
        if (llist->head->data == data)
        {
            llist->head = NULL;
            llist->tail = NULL;
        }
        return;
    }

    struct node *p = llist->head;
    if (p->data == data) //head
    {
        linked_list_pop_front(llist, &p);
        return;
    }

    while (p->next && p->next->data != data)
    {
        p = p->next;
    }
    if (!p->next)
        return;

    struct node *temp = p->next;
    if (llist->tail == temp)
        llist->tail = p;
    p->next = p->next->next;
    temp->next = NULL;
}