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

ListError ListPushHead (lst_t* list, list_elem value)
{
    if (list == nullptr)
        return NULL_PTR;
    
    if (list->free <= 0)
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
            list->free = i;
            break;
        }
    }

    if (list->free <= 0)
        return LIST_OUT;

    return IS_OK;
}

ListError ListDestroy (lst_t* list)
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
    list->head = 0;
    list->tail = 0;
    list->size = 0;

    return IS_OK;
}

ListError ListPopHead (lst_t* list, list_elem* value)
{
    if (list == nullptr)
        return NULL_PTR;

    if (list->head < 0)
        return OVERFLOW_ARRAY;
    
    if (value == nullptr)
        return NULL_PTR;

    if (list->next[list->head] == -1)
        return NO_ELEMENT;

    *value = list->data[list->head];

    list->next[list->head] = -1;
    list->prev[list->head] = -1;

    return IS_OK;
}

ListError ListPop (lst_t* list, list_elem* value, int index)
{
    if (list == nullptr)
        return NULL_PTR;
    
    if (value == nullptr)
        return NULL_PTR;

    if (list->next[index] == -1)
        return NO_ELEMENT;  

    int next_el = list->next[index];
    int prev_el = list->prev[index];

    list->size--;
    list->free = index;

    if (next_el == 0)
    {
        ListPopHead(list, value);
        return IS_OK;
    }
    
    if (prev_el == 0)
    {
        //TODO ListPopTail(list, value);
        return IS_OK;
    }
    
    *value = list->data[index];

    list->next[prev_el] = next_el;
    list->prev[next_el] = prev_el;

    list->next[index] = -1;
    list->prev[index] = -1;  
    
    return IS_OK;
}

ListError ListDump (lst_t* list)
{
    assert(list);

    printf("index ");
    for (int i = 0; i < list->head + 1; i++)
    {
        printf("  [%-d]", i);
    }
    printf("\ndata  ");

    for (int i = 0; i < list->head + 1; i++)
    {
        printf("%5.0lf", list->data[i]);
    }

    printf("\nnext  ");
    for (int i = 0; i < list->head + 1; i++)
    {
        printf("%5d", list->next[i]);
    }

    printf("\nprev  ");
    for (int i = 0; i < list->head + 1; i++)
    {
        printf("%5d", list->prev[i]);
    }

    printf("\n\nHead: %d", list->head);
    printf("\nTail: %d", list->tail);
    printf("\nCount of elemnts: %zu", list->size);
    printf("\nFree element: %d", list->free);

    printf("\n\nPtr data: [%p]", &list->data[0]);
    printf("\nPtr next: [%p]", &list->next[0]);
    printf("\nPtr prev: [%p]", &list->prev[0]);

    return IS_OK;
} 