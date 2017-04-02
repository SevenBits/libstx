// See LICENSE file for copyright and license details
#include "internal.h"

int
stxalloc(stx *sp, size_t n)
{
	memset(sp, 0, sizeof(*sp));
	if (stxgrow(sp, n))
		return -1;

	return 0;
}
