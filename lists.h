#pragma once

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>

typedef double list_elem;

#define MAX_LEN_DUMP 8

struct lst_t
{
    list_elem* data;
    int*       next;
    int*       prev;
    int        free; 
    size_t     capacity;
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
    EMPTY_ARRAY,
    BUSY_ELEM,
};

ListError ListUnit    (lst_t* list, ssize_t capacity);
ListError ListDestroy (lst_t* list);
ListError ListDump    (lst_t* list);


ListError ListPop  (lst_t* list, list_elem* value, int index);
ListError ListPush (lst_t* list, list_elem value, int index);

ListError ListPushHead (lst_t* list, list_elem value);
ListError ListPushTail (lst_t* list, list_elem value);


ListError ListPopHead  (lst_t* list, list_elem* value);
ListError ListPopTail  (lst_t* list, list_elem* vlaue);

ListError ListDumpHtml(lst_t* list);

void addHtmlName        (FILE* dot);
void addStatusInd       (FILE* dot, lst_t* list);
void addAllElements     (FILE* dot, lst_t* list);
void addMainElements    (FILE* dot, lst_t* list);
void addConnectionArray (FILE* dot, lst_t* list);
void addConnectionFree  (FILE* dot, lst_t* list);

void WriteToHtml        (lst_t* list);
bool IsValid            (lst_t* list, size_t i);





