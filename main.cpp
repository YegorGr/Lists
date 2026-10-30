#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "lists.h"


int main ()
{
    lst_t lst1 = {};

    ListError err = ListUnit(&lst1, 40);

    ListPush(&lst1, 30);
    ListPush(&lst1, 22);


    printf("%lf\n", lst1.data[1]);
    printf("%lf\n", lst1.data[2]);

    printf("%d\n", lst1.next[1]);
    printf("%d\n", lst1.next[2]);

    ListDump(&lst1);

    ListDestor(&lst1);
}

