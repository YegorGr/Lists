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
    {
        free(ptr_data);
        free(ptr_next);
        free(ptr_prev);
        return NO_MEMORY;
    }

    ptr_next[0] = 0;
    for (int i = 1; i < (int) (capacity + 1); i++)
        ptr_next[i] = -1;

    list->data     = ptr_data;
    list->next     = ptr_next;
    list->prev     = ptr_prev;
    list->free     = 1;
    list->capacity = (size_t) capacity + 1;

    list->tail     = 0;
    list->head     = 0;
    list->size     = 0;

    return IS_OK;
}

ListError ListPush (lst_t* list, list_elem value)
{
    if (list == nullptr)
        return NULL_PTR;
    
    if (free <= 0)
        return LIST_OUT;

    int elem_i = list->free;

    list->data[elem_i] = value;
    list->next[elem_i] = 0;
    list->prev[elem_i] = list->head; // бывший последний

    if (list->head == 0)      
        list->tail = list->free;
    else
        list->next[list->head] = elem_i;

    list->head = elem_i;
    list->size++; // count of elements

    list->free = 0;
    for (int i = 1; i < list->capacity; i++)
    {
        if (list->next[i] == -1)
        {
            list->free = j;
            break;
        }
    }

    if (list->free <= 0)
        return LIST_OUT;

    return IS_OK;
}

ListError ListDestor (lst_t* list)
{
    if (list == nullptr)
        return NULL_PTR;

    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = nullptr;
    list->next = nullptr;
    list->prev = nullptr;
    list->free = 0;
    list->capacity = 0;
    list->first = 0;
    list->last = 0;
    list->size = 0;

    return IS_OK;
}