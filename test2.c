#include <stdio.h>
#include "test.h"

static int my_global2;

int calc_universe(const int v) {
    return v + 42 + my_global2 + my_global3;
}
