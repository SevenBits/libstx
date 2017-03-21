// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxapp(stx *sp, const char *src, size_t len)
{
	memcpy(sp->mem + sp->len, src, len);
	stxterm(sp, sp->len + len);

	return sp;
}

stx *
stxappuni(stx *sp, uint32_t wc)
{
	int len;
	char uni8[4];
	len = stxuni8f32(uni8, wc);
	if (0 >= len)
		return NULL;
	
	if (!stxgrow(sp, len))
		return NULL;

	return stxapp(sp, uni8, len);
}
