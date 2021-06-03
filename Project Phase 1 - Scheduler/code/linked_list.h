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

void linked_list_pop_front(struct linked_list *llist, struct node *resnode)
{
    resnode = llist->head;
    llist->head = llist->head->next;
    llist->count--;
}
