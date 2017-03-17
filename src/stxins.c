// See LICENSE file for copyright and license details
#include "internal.h"

int
stxins(stx *sp, size_t pos, const char *bytes, size_t len)
{
	int err;

	if (overflow_size_add(len, 1))
		return ESTX_OVERFLOW;

	err = stxgrow(sp, len + 1);
	if (err)
		return err;

	// Create some space if inserting before the end of the buffer.
	if (pos < sp->len)
		memmove(sp->mem + pos + len, sp->mem + pos, sp->len - pos);

	memcpy(sp->mem + pos, bytes, len);
	stxterm(sp, sp->len + len);

	return 0;
}

int
stxinsuni(stx *sp, size_t pos, uint32_t wc)
{
	int len;
	char uni8[4];
	len = stxuni8f32(uni8, wc);
	if (0 > len)
		return len;

	return stxins(sp, pos, uni8, len);
}
