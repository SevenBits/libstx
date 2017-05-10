// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxcpy_mem(stx *sp, const void *src, size_t n)
{
	n = internal_min(sp->size, n);

	memcpy(sp->mem, src, n);
	sp->len = n;

	return sp;
}

stx *
stxcpy_str(stx *sp, const char *src)
{
	sp->len = internal_strncpy(sp->mem, src, sp->size);

	return sp;
}

stx *
stxcpy_spx(stx *dst, const spx src)
{
	return stxcpy_mem(dst, src.mem, src.len);
}
