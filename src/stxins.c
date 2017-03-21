// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxins(stx *sp, size_t pos, const char *bytes, size_t len)
{
	// Create some space if inserting before the end of the buffer.
	if (pos < sp->len)
		memmove(sp->mem + pos + len, sp->mem + pos, sp->len - pos);

	memcpy(sp->mem + pos, bytes, len);
	stxterm(sp, sp->len + len);

	return sp; 
}

stx *
stxinsuni(stx *sp, size_t pos, uint32_t wc)
{
	int len;
	char uni8[4];
	len = stxuni8f32(uni8, wc);
	if (0 >= len)
		return NULL;

	if (!stxgrow(sp, len))
		return NULL;

	return stxins(sp, pos, uni8, len);
}
