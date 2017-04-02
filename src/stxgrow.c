// See LICENSE file for copyright and license details
#include "internal.h"

int
stxgrow(stx *sp, size_t n)
{
	char *tmp;

	if (internal_size_add_overflows(sp->size, n)) {
		n = SIZE_MAX;
	} else {
		n = sp->size + n;
	}

	tmp = realloc(sp->mem, n);
	if (!tmp)
		return -1;

	sp->mem = tmp;
	sp->size = n;
	return 0;
}
