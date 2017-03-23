// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxcpy(stx *sp, const char *src, size_t len)
{
	memcpy(sp->mem, src, len);
	sp->len = len;

	return sp;
}
