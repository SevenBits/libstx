// See LICENSE file for copyright and license details
#include "internal.h"
#include <stdarg.h>

stx *
stxapp_mem(stx *sp, const void *src, size_t n)
{
	n = internal_min(sp->size, n);

	memmove(sp->mem + sp->len, src, n);
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
stxapp_utf8f32(stx *sp, uint32_t wc)
{
	int n = stxutf8n32(wc);
	char uni8[n];

	if (0 >= n)
		return sp;

	stxutf8f32(uni8, wc, n);

	return stxapp_mem(sp, uni8, n);
}

stx *
stxapp_spx(stx *s1, const spx s2)
{
	return stxapp_mem(s1, s2.mem, s2.len);
}
