#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "lists.h"

ListError ListUnit (lst_t* list, ssize_t capacity)
{
    assert(list);

    if (capacity < 0)
        return NEGATIVE_CAPACITY;

    list_elem* ptr_data = (list_elem*) calloc((size_t) capacity + 1, sizeof(list_elem)); // +1 for 0 in teh first
    int*       ptr_next = (int*)       calloc((size_t) capacity + 1, sizeof(int));
    int*       ptr_prev = (int*)       calloc((size_t) capacity + 1, sizeof(int));

    if (ptr_data == nullptr || ptr_next == nullptr || ptr_prev == nullptr)
        return NO_MEMORY;

    ptr_next[0] = 0;
    for (int i = 1; i < (int) (capacity + 1); i++)
        ptr_next[i] = -1;

    list->data = ptr_data;
    list->next = ptr_next;
    list->prev = ptr_prev;
    list->free = 1;
    list->capacity = (size_t) capacity + 1;

    return IS_OK;
}

ListError ListPush (lst_t* list, list_elem value)
{
    if (list == nullptr)
        return NULL_PTR;
    
    if (free <= 0)
        return LIST_OUT;

    list->data[list->free] = value;

    int free_elem = -1;
    for (int i = 0; i < list->capacity; i++)
    {
        if (list->next[i] != -1 || i == list->free)
            continue;
            
        free_elem = i;

        break;
    }

    if (free_elem <= 0)
        return LIST_OUT;
    
    list->next[list->free] = free_elem;
    list->free = free_elem;

    return IS_OK;
}

