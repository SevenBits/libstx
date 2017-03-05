// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_assign(struct oystr *s1, const char *bytes, size_t len)
{
	int err;

	if (0 != (err = oystr_ensure_size(s1, len)))
		return err;

	memcpy(s1->buf, bytes, len);
	oystr_set_len(s1, len);

	return OYSTR_OK;
}
