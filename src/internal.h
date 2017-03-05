// See LICENSE file for copyright and license details

#include "../oystr.h"
#include <stdlib.h>
#include <string.h>

static inline bool
overflow_size_add(size_t a, size_t b)
{
    return (a > (SIZE_MAX - b)) ? true : false;
}
