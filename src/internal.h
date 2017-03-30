// See LICENSE file for copyright and license details

#include "../libstx.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static inline bool
size_add_overflows(size_t a, size_t b)
{
    return (a > (SIZE_MAX - b)) ? true : false;
}
