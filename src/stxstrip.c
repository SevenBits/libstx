// See LICENSE file for copyright and license details
#include "internal.h"

stx *
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

	s1->len -= removed;
	return s1;
}

stx *
stxrstrip(stx *s1, const char *chs, size_t len)
{
	if (0 == s1->len)
		return s1;

	size_t removed = 0;
	char *begin = s1->mem + s1->len - 1;
	char *end = s1->mem;

	while (begin != end && memchr(chs, *begin, len)) {
		++removed;
		--begin;
	}

	s1->len -= removed;
	return s1;
}

stx *
stxstrip(stx *s1, const char *chs, size_t len)
{
	s1 = stxlstrip(s1, chs, len);
	s1 = stxrstrip(s1, chs, len);
	return s1;
}
