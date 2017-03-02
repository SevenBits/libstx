/* See LICENSE file for copyright and license details */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "oystr.h"

enum utf8_header {
	UTF8_H1 = 0x00,      // 1-byte header 0xxxxxxx.
	UTF8_H2 = 0x06 << 5, // 2-byte header 110xxxxx.
	UTF8_H3 = 0x0e << 4, // 3-byte header 1110xxxx.
	UTF8_H4 = 0x1e << 3, // 4-byte header 11110xxx.
	UTF8_HC = 0x02 << 6, // Continuation byte header 10xxxxxx.
};

int
oystr_utf8_from_utf32(char *bytes, uint32_t wc)
{
    int len;
    uint32_t header;
    if (wc < 0x000080) {
        header = UTF8_H1;
        len = 1;
    } else if (wc < 0x000800) {
        header = UTF8_H2;
        len = 2;
    } else if (wc < 0x010000) {
        header = UTF8_H3;
        len = 3;
    } else if (wc < 0x200000) {
        header = UTF8_H4;
        len = 4;
    } else {
        // Invalid unicode character
        return 0;
    }

    int i;
    for (i = len-1; i > 0; --i) {
        bytes[i] = (UTF8_HC | (wc & 0x3F));
        wc >>= 6;
    }
    bytes[0] = (header | wc);

    return len;
}

static inline bool
overflow_size_add(size_t a, size_t b)
{
    return (a > (SIZE_MAX - b)) ? true : false;
}

int
oystr_ensure_size(struct oystr *s1, size_t size)
{
	size_t base;
	char *tmp;

	if (overflow_size_add(size, 1))
		return OYSTR_ERR;
	size += 1;

	// Find the nearest power of two that can fit size.
	base = 2;
	if (size >= SIZE_MAX / 2) {
		base = SIZE_MAX;
	} else {
		while (base < size) {
			base <<= 1;
		}
	}

	size = base;
	if (s1->size >= size)
		// No need to increase the buffer size.
		return OYSTR_OK;

	tmp = realloc(s1->buf, sizeof(*s1->buf) * size);
	if (!tmp)
		return OYSTR_ERR;

	s1->buf = tmp;
	s1->size = size;
	return OYSTR_OK;
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
	int err;

	s1->buf = NULL;
	s1->len = 0;
	s1->size = 0;

	if (0 != (err = oystr_ensure_size(s1, size)))
		return err;

	s1->buf[s1->len] = '\0';
	return OYSTR_OK;
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
		return OYSTR_ERR;
	if (!s1->buf || s1->size < s1->len)
		return OYSTR_ERR;

	return OYSTR_OK;
}

int
oystr_set_len(struct oystr *s1, size_t len)
{
	if (len > (s1->size ? s1->size - 1 : 0)) {
			return OYSTR_ERR;
	}
	s1->len = len;
	s1->buf[s1->len]= '\0';
	return OYSTR_OK;
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
	struct oystr tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
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

	char ret = s1->buf[s1->len - 1];
	oystr_set_len(s1, s1->len - len);

	return ret;
}

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

int
oystr_write(struct oystr *s1, size_t pos, const char *bytes, size_t len)
{
	size_t newlen;

	if (pos > s1->len) {
		if (overflow_size_add(pos, len))
			return OYSTR_ERR;
		newlen = pos + len;
	} else {
		if (overflow_size_add(s1->len - pos, len))
			return OYSTR_ERR;
		newlen = (s1->len - pos + len) + 1;
	}

	int err;
	if (0 != (err = oystr_ensure_size(s1, newlen)))
		return err;

	memcpy(s1->buf + pos, bytes, len);
	oystr_set_len(s1, newlen);

	return OYSTR_OK;
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

int oystr_insert_uni(struct oystr *s1, size_t pos, uint32_t wc)
{
	int len;
	char utf8_bytes[4];
	if (0 == (len = oystr_utf8_from_utf32(utf8_bytes, wc))) {
		return OYSTR_ERR;
	}
	return oystr_insert(s1, pos, utf8_bytes, len);
}

int
oystr_append(struct oystr *s1, const char *buf, size_t len)
{
	int err;

	if (overflow_size_add(s1->len, len))
		return OYSTR_ERR;
	if (0 != (err = oystr_ensure_size(s1, s1->len + len)))
		return err;

	memcpy(s1->buf + s1->len, buf, len);
	oystr_set_len(s1, s1->len + len);

	return OYSTR_OK;
}

int oystr_append_uni(struct oystr *s1, uint32_t wc)
{
	int len;
	char utf8_bytes[4];
	if (0 == (len = oystr_utf8_from_utf32(utf8_bytes, wc))) {
		return OYSTR_ERR;
	}
	return oystr_append(s1, utf8_bytes, len);
}

void
oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end)
{
	slice->buf = s1->buf + begin;
	slice->size = s1->size - begin;
	slice->len = end - begin;
}

bool
oystr_find(struct oystr *slice, struct oystr *s1, const char *buf, size_t len)
{
	size_t i, j;

	if (0 == len)
		goto not_found;

	if (s1->len < len)
		goto not_found;

	for (i=0; i<s1->len; ++i) {
		for (j=i; j<i+len; ++j) {
			if (s1->buf[j] != buf[j-i]) {
				break;
			}
		}

		if (j-i == len)
			goto found;
	}

not_found:
	return false;

found:
	oystr_slice(slice, s1, i, j);
	return true;
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
oystr_strip(struct oystr *s1, const char *bytes, size_t len)
{
	size_t left = oystr_lstrip(s1, bytes, len);
	size_t right = oystr_rstrip(s1, bytes, len);
	if (overflow_size_add(left, right)) {
		return SIZE_MAX;
	}

	return left + right;
}
