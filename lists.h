#pragma once

#include <stdio.h>
#include <stddef.h>

typedef double list_elem;

struct lst_inf
{
    int head;
    int tail;
};

struct lst_t
{
    list_elem* data;
    int*       next;
    int*       prev;
    int        free; 
    size_t     capacity;

    lst_inf*   str_info;
};

enum ListError
{
    IS_OK = 0,
    NO_MEMORY,
    NEGATIVE_CAPACITY,
    NULL_PTR,
    LIST_OUT,
};

ListError ListUnit (lst_t* list, ssize_t capacity);
ListError ListPush (lst_t* list, list_elem value);
