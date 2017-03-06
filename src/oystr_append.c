// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_append_uni(struct oystr *s1, uint32_t wc)
{
	int len;
	char utf8_bytes[4];
	if (0 == (len = oystr_utf8_from_utf32(utf8_bytes, wc))) {
		return OYSTR_ERR;
	}
	return oystr_append(s1, utf8_bytes, len);
}

int
oystr_append(struct oystr *s1, const char *buf, size_t len)
{
	int err;

	if (overflow_size_add(s1->len, len))
		return OYSTR_OVERFLOW;
	if (0 != (err = oystr_ensure_size(s1, s1->len + len)))
		return err;

	memcpy(s1->buf + s1->len, buf, len);
	oystr_set_len(s1, s1->len + len);

	return OYSTR_OK;
}
