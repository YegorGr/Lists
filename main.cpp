#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "lists.h"


int main ()
{
    lst_t lst1 = {};

    ListError err = ListUnit(&lst1, 40);

    ListPush(&lst1, 30);

    printf("%lf\n", lst1.data[1]);
    printf("%d\n", lst1.next[1]);

}

