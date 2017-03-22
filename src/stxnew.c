// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxnew(stx *sp, size_t n)
{
	memset(sp, 0, sizeof(*sp));
	if (!stxgrow(sp, n))
		return NULL;
	*sp->mem = '\0';

	return sp;
}
