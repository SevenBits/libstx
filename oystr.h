/* See LICENSE file for copyright and license details */
#ifndef OYSTR_H
#define OYSTR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Return value for well behaved functions.
#define OYSTR_OK 0
// Return value for a malfunction.
#define OYSTR_ERR -1
// If the length field would've overflowed, it is instead set to SIZE_MAX and
// this value is returned.
#define OYSTR_OVERFLOW -2

struct oystr {
    size_t len;
    size_t size;
    char *buf;
};

static inline size_t
oystr_avail(struct oystr *s1)
{
    return s1->size ? s1->size - s1->len - 1 : 0;
}

int
oystr_ensure_size(struct oystr *s1, size_t size);

void
oystr_init(struct oystr *s1);

int
oystr_init_buf(struct oystr *s1, size_t size);

void
oystr_deinit(struct oystr *s1);

bool
oystr_valid(struct oystr *s1);

int
oystr_set_len(struct oystr *s1, size_t len);

bool
oystr_eq(const struct oystr *s1, const struct oystr *s2);

void
oystr_swap(struct oystr *s1, struct oystr *s2);

char
oystr_trunc(struct oystr *s1, size_t len);

int
oystr_assign(struct oystr *s1, const char *bytes, size_t len);

static inline int
oystr_assign_oystr(struct oystr *s1, const struct oystr *s2)
{
	return oystr_assign(s1, s2->buf, s2->len);
}

// Writing.
int
oystr_write(struct oystr *s1, size_t pos, const char *bytes, size_t len);

static inline int
oystr_write_oystr(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	return oystr_write(s1, pos, s2->buf, s2->len);
}

// Insertion.
int
oystr_insert(struct oystr *s1, size_t pos, const char *bytes, size_t len);

static inline int
oystr_insert_oystr(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	return oystr_insert(s1, pos, s2->buf, s2->len);
}

static inline int
oystr_insert_dup(struct oystr *s1, size_t pos)
{
	return oystr_insert(s1, pos, s1->buf, s1->len);
}

// Appendation.
int
oystr_append(struct oystr *s1, const char *buf, size_t len);

int
oystr_append_uni(struct oystr *s1, uint32_t wc);

static inline int
oystr_append_oystr(struct oystr *s1, const struct oystr *s2)
{
	return oystr_append(s1, s2->buf, s2->len);
}

static inline int
oystr_append_dup(struct oystr *s1)
{
	return oystr_append(s1, s1->buf, s1->len);
}

// Python inspired utilities.
void oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end);
bool oystr_find(struct oystr *slice, struct oystr *s1, const char *buf, size_t len);
size_t oystr_rstrip(struct oystr *s1, const char *bytes, size_t len);
size_t oystr_lstrip(struct oystr *s1, const char *bytes, size_t len);
size_t oystr_strip(struct oystr *s1, const char *bytes, size_t len);

// Utilities that don't necessarily operate on an oystr.
int oystr_utf8_from_utf32(char *bytes, uint32_t wc);

#endif
