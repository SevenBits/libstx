// See LICENSE file for copyright and license details
#include "internal.h"

int
stxensure_size(stx *s1, size_t size)
{
	char *tmp;
	size_t base;

	base = 2;
	if (size >= SIZE_MAX / 2) {
		base = SIZE_MAX;
	} else {
		while (base < size) {
			base <<= 1;
		}
	}

	size = base;
	if (size == s1->size)
		return 0;

	tmp = realloc(s1->mem, size);
	if (!tmp)
		return ESTX_MEMORY;

	s1->mem = tmp;
	s1->size = size;
	return 0;
}
