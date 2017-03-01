/* See LICENSE file for copyright and license details */
#include "oystr.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline bool overflow_size_add(size_t a, size_t b)
{
    return (a > (SIZE_MAX - b)) ? true : false;
}

int
oystr_ensure_size(struct oystr *s1, size_t size)
{
	size_t base;
	char *tmp;

	// Find the nearest power of two
	base = 2;
	if (size >= SIZE_MAX / 2) {
		base = SIZE_MAX;
	} else {
		while (base < size) {
			base <<= 1;
		}
	}

	if (overflow_size_add(base, 1))
		return -1;

	size = base + 1;
	if (s1->size >= size)
		return 0;

	tmp = realloc(s1->buf, sizeof(*s1->buf) * size);
	if (!tmp)
		return -1;

	s1->buf = tmp;
	s1->size = size;
	return 0;
}

int
oystr_grow(struct oystr *s1, size_t size)
{
	if (overflow_size_add(s1->len, size))
		return -1;
	return oystr_ensure_size(s1, s1->len + size);
}

void
oystr_init(struct oystr *s1)
{
	s1->buf = NULL;
	s1->len = 0;
	s1->size = 0;
}

int
oystr_init_buf(struct oystr *s1, size_t size)
{
	s1->buf = NULL;
	s1->len = 0;
	s1->size = 0;

	if (0 > oystr_ensure_size(s1, size)) {
		return -1;
	}

	s1->buf[s1->len] = '\0';
	return 0;
}

void
oystr_deinit(struct oystr *s1)
{
	free(s1->buf);
	s1->buf = NULL;
	s1->size = 0;
	s1->len = 0;
}

int 
oystr_valid(struct oystr *s1)
{
	if (!s1)
		return -1;

	if (!s1->buf || s1->size < s1->len)
		return -1;

	return 0;
}

int
oystr_set_len(struct oystr *s1, size_t len)
{
	if (len > (s1->size ? s1->size - 1 : 0)) {
			return -1;
	}
	s1->len = len;
	s1->buf[s1->len]= '\0';
	return 0;
}

bool
oystr_eq(const struct oystr *s1, const struct oystr *s2)
{
	if (s1->len != s2->len)
		return false;

	for (size_t i=0; i<s1->len; ++i) {
		if (s1->buf[i] != s2->buf[i])
			return false;
	}

	return true;
}

void
oystr_swap(struct oystr *s1, struct oystr *s2)
{
	s1->buf = s2->buf;
	s1->len = s2->len;
	s1->size = s2->size;
}

char
oystr_trunc(struct oystr *s1, size_t len)
{
	if (len > s1->len) {
		oystr_set_len(s1, 0);
		return '\0';
	}

	if (0 == s1->len)
		return '\0';

	char ret = s1->buf[s1->len];
	oystr_set_len(s1, s1->len - len);

	return ret;
}

int
oystr_write(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	//TODO determine length here
	int err = oystr_ensure_size(s1, s2->len);
	if (0 > err)
		return err;

	memcpy(s1->buf + pos, s2->buf, s2->len);
	oystr_set_len(s1, 0);

	return 0;
}

int
oystr_insert(struct oystr *s1, size_t pos, const char *bytes, size_t len)
{
	if (overflow_size_add(s1->len, len))
		return -1;
	int err = oystr_ensure_size(s1, s1->len + len);
	if (0 > err)
		return err;

	// Create some space if inserting before the end of the buffer.
	if (pos < s1->len)
		memcpy(s1->buf + pos + len, s1->buf + pos, s1->len - pos);

	memcpy(s1->buf + pos, bytes, len);
	oystr_set_len(s1, s1->len + len);
	return 0;
}

int
oystr_append(struct oystr *s1, const char *buf, size_t len)
{
	if (overflow_size_add(s1->len, len))
		return -1;
	int err = oystr_ensure_size(s1, s1->len + len);
	if (0 > err)
		return err;

	memcpy(s1->buf + s1->len, buf, len);
	oystr_set_len(s1, s1->len + len);

	return 0;
}

bool
oystr_find(struct oystr *slice,
	const struct oystr *s1,
	const char *buf,
	size_t len)
{
	if (0 == len)
		goto not_found;

	size_t i, j;

	for (i=0; i<s1->len; ++i) {
		if (i > len)
			// @s1 cannot possibly contain @buf
			goto not_found;

		for (j=0; j<len; ++j) {
			if (s1->buf[j] != buf[j]) {
				break;
			}
		}

		if (j == len-1)
			goto found;
	}

not_found:
	return false;

found:
	slice->buf = s1->buf + i;
	slice->len = len;
	slice->size = s1->size - i;
	return true;
}

size_t
oystr_lstrip(struct oystr *s1, const char *bytes, size_t len)
{
	size_t removed = 0;
	char *begin = s1->buf;
	char *end = s1->buf + s1->len;

	while (begin != end && memchr(bytes, len, *begin)) {
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
	size_t removed = 0;
	char *begin = s1->buf + s1->len;
	char *end = s1->buf;

	while (begin != end && memchr(bytes, len, *begin)) {
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

void
oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end)
{
	slice->buf = s1->buf + begin;
	slice->size = s1->size - begin;
	slice->len = end - begin;
}
