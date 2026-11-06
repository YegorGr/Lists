#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>

#include "lists.h"

ListError ListUnit (lst_t* list, ssize_t capacity)
{
    assert(list);

    if (capacity < 0)
        return NEGATIVE_CAPACITY;

    list_elem* ptr_data = (list_elem*) calloc((size_t) capacity + 1, sizeof(list_elem));
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
    ptr_prev[0] = 0;

    for (size_t i = 1; i <= (size_t)capacity; i++)
    {
        ptr_next[i] = (i < (size_t)capacity) ? (int)(i + 1) : 0;
        ptr_prev[i] = -1;
    }

    list->data     = ptr_data;
    list->next     = ptr_next;
    list->prev     = ptr_prev;
    list->free     = (capacity > 0) ? 1 : 0;
    list->capacity = (size_t) capacity + 1;
    list->size     = 0;

    return IS_OK;
}

ListError ListDestroy (lst_t* list)
{
    if (list == nullptr)
        return NULL_PTR;

    free(list->data);
    free(list->next);
    free(list->prev);

    list->data     = nullptr;
    list->next     = nullptr;
    list->prev     = nullptr;
    list->free     = 0;
    list->capacity = 0;
    list->size     = 0;

    return IS_OK;
}

ListError ListPush (lst_t* list, list_elem value, int index)
{
    if (list == nullptr)
        return NULL_PTR;

    if (index < 0 || (size_t) index >= list->capacity || list->prev[index] == -1)
        return NO_ELEMENT;

    if (list->free <= 0)
        return LIST_OUT;

    int i      = list->free;
    list->free = list->next[i];

    int next_el = list->next[index];

    list->data[i] = value;
    list->prev[i] = index;
    list->next[i] = next_el;

    list->next[index]   = i;
    list->prev[next_el] = i;

    list->size++;

    return IS_OK;
}

ListError ListPushHead (lst_t* list, list_elem value)
{
    if (list == nullptr)
        return NULL_PTR;

    return ListPush(list, value, list->prev[0]);
}

ListError ListPushTail (lst_t* list, list_elem value)
{
    if (list == nullptr)
        return NULL_PTR;

    return ListPush(list, value, 0);
}

ListError ListPop (lst_t* list, list_elem* value, int index)
{
    if (list == nullptr || value == nullptr)
        return NULL_PTR;

    if (index <= 0 || (size_t)index >= list->capacity || list->prev[index] == -1)
        return NO_ELEMENT;

    *value = list->data[index];

    int next_el = list->next[index];
    int prev_el = list->prev[index];

    list->next[prev_el] = next_el;
    list->prev[next_el] = prev_el;

    list->data[index] = 0;
    list->prev[index] = -1;
    list->next[index] = list->free;
    list->free        = index;

    list->size--;

    return IS_OK;
}

ListError ListPopHead (lst_t* list, list_elem* value)
{
    if (list == nullptr)
        return NULL_PTR;

    return ListPop(list, value, list->prev[0]);
}

ListError ListPopTail (lst_t* list, list_elem* value)
{
    if (list == nullptr)
        return NULL_PTR;

    return ListPop(list, value, list->next[0]);
}

ListError ListDump (lst_t* list)
{
    assert(list);

    printf("index ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printf("  [%-zu]", i);
    }
    printf("\ndata  ");

    for (size_t i = 0; i < list->capacity; i++)
    {
        printf("%5.0lf", list->data[i]);
    }

    printf("\nnext  ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printf("%5d", list->next[i]);
    }

    printf("\nprev  ");
    for (size_t i = 0; i < list->capacity; i++)
    {
        printf("%5d", list->prev[i]);
    }

    printf("\n\nTail (first): %d", list->next[0]);
    printf("\nHead (last):  %d", list->prev[0]);
    printf("\nCount of elements: %zu", list->size);
    printf("\nFree element: %d", list->free);

    printf("\n\nPtr data: [%p]", (void*)&list->data[0]);
    printf("\nPtr next: [%p]", (void*)&list->next[0]);
    printf("\nPtr prev: [%p]\n", (void*)&list->prev[0]);

    return IS_OK;
}

ListError ListDumpHtml(lst_t* list)
{
    assert(list);

    FILE* dot = fopen("dump.dot", "w");
    if (!dot) 
        return NULL_PTR;

    addHtmlName       (dot);
    addStatusInd      (dot, list);
    addAllElements    (dot, list);
    addMainElements   (dot, list);
    addConnectionArray(dot, list);
    addConnectionFree (dot, list);

    fprintf(dot, "}\n");
    fclose(dot);

    WriteToHtml(list);

    return IS_OK;
}

void addHtmlName (FILE* dot)
{
   fprintf(dot, "digraph G {\n"
                 "  rankdir=TB;\n"
                 "  ranksep=0.35;\n"
                 "  node [fontname=\"Calibri\", fontsize=14];\n\n");

    fprintf(dot, "  table_node [shape=plaintext, label=<\n"
                 "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"5\">\n"
                 "      <TR><TD><B>idx</B></TD><TD><B>data</B></TD><TD><B>next</B></TD><TD><B>prev</B></TD><TD><B>status</B></TD></TR>\n");
}

void addStatusInd (FILE* dot, lst_t* list)
{
    for (size_t i = 0; i < list->capacity; i++)
    {
        const char* status = nullptr;

        if (!IsValid(list, i))
            status = "<FONT COLOR=\"red\"><B>ERROR</B></FONT>";

        else if (i == 0)
            status = "SENTINEL";

        else if (list->prev[i] == -1)
            status = "FREE";
        else
            status = "USED";
        
        double print_data = 3.14;
        if (i == 0 || list->prev[i] == -1)
            print_data = 0.0;
        else 
            print_data = list->data[i];
        
        fprintf(dot, "      <TR><TD>%zu</TD><TD>%.0lf</TD><TD>%d</TD><TD>%d</TD><TD>%s</TD></TR>\n",
                i, print_data, list->next[i], list->prev[i], status);
    }
    fprintf(dot, "    </TABLE>\n  >];\n\n");
}

void addConnectionArray (FILE* dot, lst_t* list)
{
    for (size_t i = 0; i < list->capacity; i++)
    {
        if (i != 0 && list->prev[i] == -1) // skip free 
            continue;

        int next_elem = list->next[i];
        int prev_elem = list->prev[i];

        bool next_ok = (next_elem >= 0 && next_elem < list->capacity && list->prev[next_elem] == i);
        const char* next_color = next_ok ? "#1e88e5" : "#ff0000"; 

        if (next_elem >= 0 && next_elem < list->capacity)
        {
            if (next_elem != 0)
                fprintf(dot, "  node_%zu:n -> node_%d [rank=same, weight=100, color=\"%s\", penwidth=1.5];\n", 
                        i, next_elem, next_color);
            else if (i != 0 || !next_ok)
                fprintf(dot, "  node_%zu:n -> node_0 [rank=same, constraint=false, color=\"%s\", penwidth=1.5];\n", 
                        i, next_color);
        }
        else
        {
            fprintf(dot, "  bad_next_%zu [shape=octagon, style=filled, fillcolor=\"#ff4d4d\", label=\"BAD NEXT: %d\"];\n", i, next_elem);
            fprintf(dot, "  node_%zu:n -> bad_next_%zu [color=\"#ff0000\", penwidth=3];\n", i, i);
        }

        if (list->size > 0 || i != 0)
        {
            bool prev_ok = (prev_elem >= 0 && prev_elem < list->capacity && list->next[prev_elem] == i);
            const char* prev_color = prev_ok ? "#43a047" : "#ff0000"; 

            if (prev_elem >= 0 && prev_elem < list->capacity)
            {
                fprintf(dot, "  node_%zu:p -> node_%d [rank=same, color=\"%s\", constraint=false, style=dashed, penwidth=1.5];\n", 
                        i, prev_elem, prev_color);
            }
            else
            {
                fprintf(dot, "  bad_prev_%zu [shape=octagon, style=filled, fillcolor=\"#ff4d4d\", label=\"BAD PREV: %d\"];\n", i, prev_elem);
                fprintf(dot, "  node_%zu:p -> bad_prev_%zu [color=\"#ff0000\", style=dashed, penwidth=3];\n", i, i);
            }
        }
    }
}


void addAllElements (FILE* dot, lst_t* list)
{
    for (size_t i = 0; i < list->capacity; i++)
    {
        const char* color = "#2f7bfe"; // used

        if (!IsValid(list, i))
            color = "#ff4d4d"; // error

        else if (i == 0)                     
            color = "#dd8007"; // sentinel
        
        else if (list->prev[i] == -1)   
            color = "#57f157"; // free
        
        else if (i == list->next[0])        
            color = "#ff4af3"; // tail
        
        else if (i == list->prev[0])        
            color = "#9372f6"; // head

        char data_str[MAX_LEN_DUMP] = {};
        if (i != 0 && list->prev[i] == -1)
            sprintf(data_str, "FREE");
        else
            sprintf(data_str, "%.0lf", list->data[i]);

        fprintf(dot, "  node_%zu [shape=record, style=filled, fillcolor=\"%s\", "
                     "label=\"{ INDEX = %zu | DATA = %s | { <n> NEXT = %d | <p> PREV = %d | meow } }\"];\n",
                i, color, i, data_str, list->next[i], list->prev[i]);
    }

    fprintf(dot, "\n  node [shape=box, style=\"rounded,filled\"];\n"
                 "  FREE [fillcolor=\"#57f157\"];\n"
                 "  TAIL [fillcolor=\"#ff4af3\"];\n"
                 "  HEAD  [fillcolor=\"#9372f6\"];\n"
                 "  ERROR [fillcolor=\"#ff4d4d\"];\n\n");
}

void addMainElements (FILE* dot, lst_t* list)
{
    int tail = list->next[0];
    int head = list->prev[0];

    if (list->free > 0)
    {
        fprintf(dot, "  { rank=same; FREE; node_%d; }\n", list->free);
        fprintf(dot, "  FREE -> node_%d;\n", list->free);
    }

    if (tail >= 0) 
    {
        fprintf(dot, "  { rank=same; TAIL; node_%d; }\n", tail);
        fprintf(dot, "  TAIL -> node_%d;\n", tail);
    }
    
    if (head >= 0) 
    {
        fprintf(dot, "  { rank=same; HEAD; node_%d; }\n", head);
        fprintf(dot, "  HEAD -> node_%d;\n\n", head);
    }
}

void addConnectionFree (FILE* dot, lst_t* list)
{
    int free_elem = list->free;
    while (free_elem != 0)
    {
        int next_elem = list->next[free_elem];
        
        if (next_elem != 0) 
            fprintf(dot, "  node_%d:n -> node_%d [weight=100];\n", free_elem, next_elem);

        else 
            fprintf(dot, "  node_%d:n -> node_0 [weight=100];\n", free_elem);
        
        free_elem = next_elem;
    }  
}

void WriteToHtml (lst_t* list)
{
    FILE* html = fopen("log.html", "a");

    if (html) 
    {
        fclose(html);
    }

    system("dot -Tpng dump.dot >> log.html");
} 

bool IsValid (lst_t* list, size_t i)
{
    if (i != 0 && list->prev[i] == -1)
        return (list->next[i] >= 0 && list->next[i] < list->capacity);

    int next_elem = list->next[i];
    int prev_elem = list->prev[i];

    if (next_elem < 0 || next_elem >= list->capacity)
        return false;
    
    if (prev_elem < 0 || prev_elem >= list->capacity)
        return false;

    if (list->prev[next_elem] != i)
        return false;

    if (list->next[prev_elem] != i)
        return false;

    return true;
}