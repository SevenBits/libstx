// See LICENSE file for copyright and license details
#include "internal.h"

size_t
stxlstrip(stx *s1, const char *chs, size_t len)
{
	size_t removed = 0;
	char *begin = s1->mem;
	char *end = s1->mem + s1->len;

	while (begin != end && memchr(chs, *begin, len)) {
		++removed;
		++begin;
	}

	if (begin != end)
		memmove(s1->mem, begin, s1->len - removed);

	stxterm(s1, s1->len - removed);
	return removed;
}

size_t
stxrstrip(stx *s1, const char *chs, size_t len)
{
	if (0 == s1->len)
		return 0;

	size_t removed = 0;
	char *begin = s1->mem + s1->len - 1;
	char *end = s1->mem;

	while (begin != end && memchr(chs, *begin, len)) {
		++removed;
		--begin;
	}

	stxterm(s1, s1->len - removed);
	return removed;
}

size_t
stxstrip(stx *s1, const char *chs, size_t len)
{
	size_t left = stxlstrip(s1, chs, len);
	size_t right = stxrstrip(s1, chs, len);
	if (size_add_overflows(left, right)) {
		return SIZE_MAX;
	}

	return left + right;
}
