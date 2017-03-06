// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_ensure_size(struct oystr *s1, size_t size)
{
	size_t base;
	char *tmp;

	if (overflow_size_add(size, 1))
		return OYSTR_OVERFLOW;
	size += 1;

	// Find the nearest power of two that can fit size.
	base = 2;
	if (size >= SIZE_MAX / 2) {
		base = SIZE_MAX;
	} else {
		while (base < size) {
			base <<= 1;
		}
	}

	size = base;
	if (s1->size >= size)
		// No need to increase the buffer size.
		return OYSTR_OK;

	tmp = realloc(s1->buf, sizeof(*s1->buf) * size);
	if (!tmp)
		return OYSTR_ERR;

	s1->buf = tmp;
	s1->size = size;
	return OYSTR_OK;
}
