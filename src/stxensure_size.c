// See LICENSE file for copyright and license details
#include "internal.h"

int
stxensure_size(stx *s1, size_t size)
{
	char *tmp;

	if (s1->size >= size)
		return 0;

	tmp = realloc(s1->mem, size);
	if (!tmp)
		return -1;

	s1->mem = tmp;
	s1->size = size;
	return 0;
}
