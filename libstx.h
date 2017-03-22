/* See LICENSE file for copyright and license details */
#ifndef STX_H
#define STX_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct stx {
    size_t len;
    size_t size;
    char *mem;
};

typedef struct stx stx;

static inline size_t
stxavail(stx *sp)
{
    return sp->size ? sp->size - sp->len - 1 : 0;
}

// NOTE: 'n' must be greater than zero, otherwise behavior is undefined.
stx *stxnew(stx *sp, size_t n);
stx *stxdel(stx *sp);

stx *stxgrow(stx *sp, size_t n);
stx *stxensure_size(stx *sp, size_t n);
stx *stxvalid(stx *sp);

// Various utility
stx *stxterm(stx *sp, size_t len);
bool stxeq(const stx *s1, const stx *s2);
void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t len);
stx *stxcpy(stx *sp, const char *src, size_t len);
stx *stxdup(stx *sp, const stx *src);

// Insertion
stx *stxins(stx *sp, size_t pos, const char *src, size_t len);
stx *stxinsuni(stx *sp, size_t pos, uint32_t wc);

static inline stx *
stxinsstx(stx *s1, size_t pos, stx *s2)
{
	return stxins(s1, pos, s2->mem, s2->len);
}

static inline stx *
stxinsdup(stx *sp, size_t pos)
{
	return stxins(sp, pos, sp->mem, sp->len);
}

// Appendation.
stx *stxapp(stx *sp, const char *src, size_t len);
stx *stxappuni(stx *sp, uint32_t wc);

static inline stx *
stxappstx(stx *s1, const stx *s2)
{
	return stxapp(s1, s2->mem, s2->len);
}

static inline stx *
stxappdup(stx *sp)
{
	return stxapp(sp, sp->mem, sp->len);
}

// Python inspired utilities.
stx *stxslice(stx *slice, stx *sp, size_t begin, size_t end);
stx *stxfind(stx *slice, stx *sp, const char *src, size_t len);
stx *stxrstrip(stx *sp, const char *chs, size_t len);
stx *stxlstrip(stx *sp, const char *chs, size_t len);
stx *stxstrip(stx *sp, const char *chs, size_t len);

// Other library utilites
int stxuni8f32(char *dst, uint32_t wc);

#endif
