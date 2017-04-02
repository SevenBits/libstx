// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxins_mem(stx *sp, size_t pos, const char *src, size_t len)
{
	// Create some space if inserting before the end of the buffer.
	if (pos < sp->len)
		memmove(sp->mem + pos + len, sp->mem + pos, sp->len - pos);

	memcpy(sp->mem + pos, src, len);
	sp->len += len;

	return sp; 
}

stx *
stxins_str(stx *sp, size_t pos, const char *src)
{
	return stxins_mem(sp, pos, src, strlen(src));
}

stx *
stxins_uni(stx *sp, size_t pos, uint32_t wc)
{
	int len;
	char uni8[4];
	len = stxuni8f32(uni8, wc);
	if (0 >= len)
		return NULL;

	return stxins_mem(sp, pos, uni8, len);
}

stx *
stxins_stx(stx *s1, size_t pos, const stx *s2)
{
	return stxins_mem(s1, pos, s2->mem, s2->len);
}
