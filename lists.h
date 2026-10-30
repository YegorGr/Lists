#pragma once

#include <stdio.h>
#include <stddef.h>

typedef double list_elem;


struct lst_t
{
    list_elem* data;
    int*       next;
    int*       prev;
    int        free; 
    size_t     capacity;

    int        tail; //самый первый (который сейчас)
    int        head; // бывший последний (который последний сейчас)
    size_t     size; 
};

enum ListError
{
    IS_OK = 0,
    NO_MEMORY,
    NEGATIVE_CAPACITY,
    NULL_PTR,
    LIST_OUT,
    OVERFLOW_ARRAY,
    NO_ELEMENT,
};

ListError ListUnit   (lst_t* list, ssize_t capacity);
ListError ListDestor (lst_t* list);
ListError ListDump   (lst_t* list);


ListError ListPop  (lst_t* list, list_elem* value, int index);
ListError ListPush (lst_t* list, list_elem* value, int index);

ListError ListPushHead (lst_t* list, list_elem value);
ListError ListPushTail (lst_t* list, list_elem value);


ListError ListPopHead  (lst_t* list, list_elem* value);
ListError ListPopTail  (lst_t* list, list_elem* vlaue);

