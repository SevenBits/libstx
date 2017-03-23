// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxtrunc(stx *sp, size_t len)
{
	if (len >= sp->len) {
		sp->len = 0;
		return sp;
	}

	sp->len -= len;
	return sp;
}
