/* See LICENSE file for copyright and license details */
#ifndef OYSTR_H
#define OYSTR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Return value for well behaved functions.
#define OYSTR_OK 0
// Return value for any malfunction.
#define OYSTR_ERR -1

/**
 * The oystr structure for holding multibyte data.
 *
 * All of the fields in this struct are public and available for
 * direct modification and assignment, just don't expect the oystr functions
 * to behave correctly if you don't follow the same rules as the rest of the
 * library when it comes to setting these fields.
 */
struct oystr {
    size_t len;
    size_t size;
    char *buf;
};

/**
 * Check how much space is available in @s1->buf.
 *
 * The space leftover is the amount of uninitialized memory after
 * s1->buf+s1->len+1.
 */
static inline size_t oystr_avail(struct oystr *s1)
{
    return s1->size ? s1->size - s1->len - 1 : 0;
}

/**
 * Ensure that @s1-buf can hold at least @size bytes.
 *
 * Please note that @s1->buf will most likely be larger than @size,
 * as this function allocates enough for @size + 1, and grows the buffer
 * as powers of two.
 */
int
oystr_ensure_size(struct oystr *s1, size_t size);

/**
 * Initialize @s1's fields to be 0.
 */
void
oystr_init(struct oystr *s1);

/**
 * Initialize @s1's fields to be 0, then allocate memory for @s1->buf.
 *
 * The memory allocated for @s1->buf is uninitialized. Returns ISTR_ERR if the
 * allocation fails, otherwise returns ISTR_OK.
 */
int
oystr_init_buf(struct oystr *s1, size_t size);

/**
 * Deinitialze @s1 by freeing all allocated memory and setting fields to 0.
 *
 * @s1->buf must either be NULL or malloc'd memory, otherwise behavior is
 * undefined.
 */
void
oystr_deinit(struct oystr *s1);

/**
 * Validate @s1 to make sure it is a properly initialized and allocated oystr.
 *
 * This test is useful if dealing with oystr's where the caller may not be sure
 * if the oystr is initialized properly or not. Returns true if
 * @s1->size > 0, @s1->len < @s1->size, and @s1->buf != NULL, otherwise 
 * returns false.
 */
bool
oystr_valid(struct oystr *s1);

/**
 * Sets @s1.len to @len and NULL-terminates the string after this length.
 *
 * Returns ISTR_ERR If @len is greater than @s1.size - 1, otherwise returns
 * ISTR_OK.
 */
int
oystr_set_len(struct oystr *s1, size_t len);

/**
 * Compare two oystrs for equality.
 *
 * Returns false if the contents of @s1.buf are not the same as @s1.buf,
 * otherwise returns true.
 */
bool
oystr_eq(const struct oystr *s1, const struct oystr *s2);

/**
 * Swap the contents of two oystrs.
 *
 * This function is equivalent to just assigning two structs to one another
 * while using a temporary struct as a buffer.
 */
void
oystr_swap(struct oystr *s1, struct oystr *s2);

/**
 * Truncate @s1 by @len bytes.
 *
 * Returns the last character truncated. If @len > @s1.len, @s1.len is set to 0
 * and '\0' is returned.
 */
char
oystr_trunc(struct oystr *s1, size_t len);

/**
 * Assign the contents of @bytes to @s1.
 *
 * If @s1.buf doesn't not contain enough room to hold @bytes, @s1.buf will be
 * reallocated. Returns ISTR_ERR upon any reallocation error, otherwise returns
 * ISTR_OK.
 */
int
oystr_assign(struct oystr *s1, const char *bytes, size_t len);

/**
 * Assign the contents of @s2 to @s1.
 * 
 * A convenience wrapper around oystr_assign.
 */
static inline int
oystr_assign_oystr(struct oystr *s1, const struct oystr *s2)
{
	return oystr_assign(s1, s2->buf, s2->len);
}

/**
 * Write @bytes directly over @s1 starting at index @pos.
 *
 * This function overwrites bytes directly, and doesn't make any calls to
 * memmove. If the bytes being written would extend beyond @s1.buf, then @s1.buf
 * is reallocated.
 *
 * Returns ISTR_ERR upon allocation failure, otherwise ISTR_OK.
 */
int
oystr_write(struct oystr *s1, size_t pos, const char *bytes, size_t len);

/**
 * Write @s2 directly over @s1 starting at index @pos.
 *
 * A convenience wrapper around oystr_write.
 */
static inline int
oystr_write_oystr(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	return oystr_write(s1, pos, s2->buf, s2->len);
}

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

void
oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end);

bool
oystr_find(struct oystr *slice, struct oystr *s1, const char *buf, size_t len);

size_t
oystr_rstrip(struct oystr *s1, const char *bytes, size_t len);

size_t
oystr_lstrip(struct oystr *s1, const char *bytes, size_t len);

size_t
oystr_strip(struct oystr *s1, const char *bytes, size_t len);

int
oystr_utf8_from_utf32(char *bytes, uint32_t wc);

#endif
