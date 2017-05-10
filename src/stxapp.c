// See LICENSE file for copyright and license details
#include "internal.h"
#include <stdarg.h>

stx *
stxapp_mem(stx *sp, const void *src, size_t n)
{
	n = internal_min(sp->size, n);

	memcpy(sp->mem + sp->len, src, n);
	sp->len += n;

	return sp;
}

stx *
stxapp_str(stx *sp, const char *src)
{
	sp->len += internal_strncpy(sp->mem + sp->len, src, sp->size - sp->len);
	return sp;
}

stx *
stxapp_u32(stx *sp, uint32_t wc)
{
	if (0 >= stxuni8f32(sp->mem + sp->len, wc))
		return NULL;

	return sp;
}

stx *
stxapp_spx(stx *s1, const spx s2)
{
	return stxapp_mem(s1, s2.mem, s2.len);
}
