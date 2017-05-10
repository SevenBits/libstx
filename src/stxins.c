// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxins_mem(stx *sp, size_t pos, const void *src, size_t n)
{
	n = internal_min(sp->size, n);

	// Create some space if inserting before the end of the buffer.
	if (pos < sp->len)
		memmove(sp->mem + pos + n, sp->mem + pos, sp->len - pos);

	memcpy(sp->mem + pos, src, n);
	sp->len += n;

	return sp; 
}

stx *
stxins_str(stx *sp, size_t pos, const char *src)
{
	if (!src)
		return sp;

	return stxins_mem(sp, pos, src, strlen(src));
}

stx *
stxins_u32(stx *sp, size_t pos, uint32_t wc)
{
	int n;
	char uni8[4];
	n = stxuni8f32(uni8, wc);
	if (0 >= n)
		return NULL;

	return stxins_mem(sp, pos, uni8, n);
}

stx *
stxins_spx(stx *s1, size_t pos, const spx s2)
{
	return stxins_mem(s1, pos, s2.mem, s2.len);
}
