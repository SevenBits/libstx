// See LICENSE file for copyright and license details
#include "internal.h"

int
stxgrow(stx *sp, size_t n)
{
	if (internal_size_add_overflows(sp->size, n)) {
		n = SIZE_MAX;
	} else {
		n = sp->size + n;
	}

	if (!sp->mem && !n) {
		memset(sp, 0, sizeof(*sp));
	} else {
		char *tmp = realloc(sp->mem, n);

		if (!tmp)
			return -1;

		sp->mem = tmp;
		sp->size = n;
	}

	return 0;
}
