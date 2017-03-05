// See LICENSE file for copyright and license details
#include "internal.h"

size_t
oystr_lstrip(struct oystr *s1, const char *bytes, size_t len)
{
	size_t removed = 0;
	char *begin = s1->buf;
	char *end = s1->buf + s1->len;

	while (begin != end && memchr(bytes, *begin, len)) {
		++removed;
		++begin;
	}

	if (begin != end)
		memmove(s1->buf, begin, s1->len - removed);

	oystr_set_len(s1, s1->len - removed);
	return removed;
}

size_t
oystr_rstrip(struct oystr *s1, const char *bytes, size_t len)
{
	if (0 == s1->len)
		return OYSTR_OK;

	size_t removed = 0;
	char *begin = s1->buf + s1->len - 1;
	char *end = s1->buf;

	while (begin != end && memchr(bytes, *begin, len)) {
		++removed;
		--begin;
	}

	oystr_set_len(s1, s1->len - removed);
	return removed;
}

size_t
oystr_strip(struct oystr *s1, const char *bytes, size_t len)
{
	size_t left = oystr_lstrip(s1, bytes, len);
	size_t right = oystr_rstrip(s1, bytes, len);
	if (overflow_size_add(left, right)) {
		return SIZE_MAX;
	}

	return left + right;
}
