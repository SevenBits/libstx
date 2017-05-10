// See LICENSE file for copyright and license details
#include "internal.h"

int
stxensuresize(stx *sp, size_t n)
{
	if (sp->size >= n)
		return 0;

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
