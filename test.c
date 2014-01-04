#include <stdio.h>
#include "test.h"

static int my_global1 = 666;
int my_global3 = 1234;

static void foo(void)
{
    int x = 2;
    printf("%d\n", x);
}

int main(void)
{
    int universe = calc_universe(0);
    printf("Hello! The universe from 0 is: %d\n", universe);
    return 0;
}
