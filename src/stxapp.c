// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxapp(stx *sp, const char *src, size_t len)
{
	memcpy(sp->mem + sp->len, src, len);
	sp->len += len;

	return sp;
}

stx *
stxappuni(stx *sp, uint32_t wc)
{
	if (0 >= stxuni8f32(sp->mem + sp->len, wc))
		return NULL;

	return sp;
}
