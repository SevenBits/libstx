// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxgrow(stx *s1, size_t n)
{
	char *tmp;

	if (size_add_overflows(s1->size, n))
		return NULL;

	tmp = realloc(s1->mem, s1->size + n);
	if (!tmp)
		return NULL;

	s1->mem = tmp;
	s1->size += n;
	return s1;
}
