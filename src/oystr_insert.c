// See LICENSE file for copyright and license details
#include "internal.h"

int
oystr_insert_uni(struct oystr *s1, size_t pos, uint32_t wc)
{
	int len;
	char utf8_bytes[4];
	if (0 == (len = oystr_utf8_from_utf32(utf8_bytes, wc))) {
		return OYSTR_ERR;
	}
	return oystr_insert(s1, pos, utf8_bytes, len);
}

int
oystr_insert(struct oystr *s1, size_t pos, const char *bytes, size_t len)
{
	int err;

	if (overflow_size_add(s1->len, len))
		return OYSTR_ERR;
	if (0 != (err = oystr_ensure_size(s1, s1->len + len)))
		return err;

	// Create some space if inserting before the end of the buffer.
	if (pos < s1->len)
		memmove(s1->buf + pos + len, s1->buf + pos, s1->len - pos);

	memcpy(s1->buf + pos, bytes, len);
	oystr_set_len(s1, s1->len + len);

	return OYSTR_OK;
}
