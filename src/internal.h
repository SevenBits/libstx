// See LICENSE file for copyright and license details
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../libstx.h"

static inline bool
internal_size_add_overflows(size_t a, size_t b)
{
	return a > SIZE_MAX - b ? true : false;
}

static inline size_t
internal_min(size_t a, size_t b)
{
	return a > b ? b : a;
}

static inline size_t
internal_strncpy(char *str, const char *src, size_t max)
{
	size_t i = 0;
	while (i < max && *src) {
		str[i] = *src;
		++src;
		++i;
	}

	return i;
}
