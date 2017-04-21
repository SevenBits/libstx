// See LICENSE file for copyright and license details
#include "internal.h"
#include <stdarg.h>

stx *
stxapp_mem(stx *sp, const char *src, size_t len)
{
	memcpy(sp->mem + sp->len, src, len);
	sp->len += len;

	return sp;
}

stx *
stxapp_str(stx *sp, const char *src)
{
	sp->len += internal_strncpy(sp->mem + sp->len, src, sp->size - sp->len);
	return sp;
}

stx *
stxapp_uni(stx *sp, uint32_t wc)
{
	if (0 >= stxuni8f32(sp->mem + sp->len, wc))
		return NULL;

	return sp;
}

stx *
stxapp_stx(stx *s1, const stx *s2)
{
	return stxapp_mem(s1, s2->mem, s2->len);
}
