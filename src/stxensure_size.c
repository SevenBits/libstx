// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxensure_size(stx *s1, size_t size)
{
	char *tmp;

	if (s1->size >= size)
		return s1;

	tmp = realloc(s1->mem, size);
	if (!tmp)
		return NULL;

	s1->mem = tmp;
	s1->size = size;
	return s1;
}
