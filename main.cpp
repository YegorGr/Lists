#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "lists.h"


int main ()
{
    lst_t lst1 = {};

    ListError err = ListUnit(&lst1, 20);
    double value = 0;
    ListPushHead(&lst1, 11);
    ListPushHead(&lst1, 22);    
    ListPushHead(&lst1, 33);
    ListPushHead(&lst1, 44);
    ListDumpHtml(&lst1, "хуйня");

    ListPop(&lst1, &value, 2);

    ListPush(&lst1, 55.0, 1);

    ListDump(&lst1);

    ListDestroy(&lst1);
}

