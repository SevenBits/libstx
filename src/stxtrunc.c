// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxtrunc(stx *sp, size_t len)
{
	if (len > sp->len) {
		stxterm(sp, 0);
		return NULL;
	}

	if (0 == sp->len)
		return NULL;

	stxterm(sp, sp->len - len);
	return sp;
}
