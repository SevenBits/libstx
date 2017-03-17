/* See LICENSE file for copyright and license details */
#ifndef STX_H
#define STX_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Error return values
#define ESTX_MEMORY -1
#define ESTX_OVERFLOW -2
#define ESTX_INVALID_UNICODE -3

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
int stxnew(stx *sp, size_t n);
void stdel(stx *sp);

int stxgrow(stx *sp, size_t n);
int stxensure_size(stx *sp, size_t n);
bool stxvalid(stx *sp);

// Various utility
int stxterm(stx *sp, size_t len);
bool stxeq(const stx *s1, const stx *s2);
void stxswap(stx *s1, stx *s2);
char stxtrunc(stx *sp, size_t len);
int stxcpy(stx *sp, const char *src, size_t len);
int stxdup(stx *sp, const stx *src);

// Insertion
int stxins(stx *sp, size_t pos, const char *src, size_t len);
int stxinsuni(stx *sp, size_t pos, uint32_t wc);

static inline int
stxinsstx(stx *s1, size_t pos, stx *s2)
{
	return stxins(s1, pos, s2->mem, s2->len);
}

static inline int
stxinsdup(stx *sp, size_t pos)
{
	return stxins(sp, pos, sp->mem, sp->len);
}

// Appendation.
int stxapp(stx *sp, const char *src, size_t len);
int stxappuni(stx *sp, uint32_t wc);

static inline int
stxappstx(stx *s1, const stx *s2)
{
	return oystr_append(s1, s2->mem, s2->len);
}

static inline int
stxappdup(stx *sp)
{
	return stxapp(sp, sp->mem, sp->len);
}

// Python inspired utilities.
void stxslice(stx *slice, stx *sp, size_t begin, size_t end);
bool stxfind(stx *slice, stx *sp, const char *src, size_t len);
size_t stxrstrip(stx *sp, const char *chs, size_t len);
size_t stxlstrip(stx *sp, const char *chs, size_t len);
size_t stxstrip(stx *sp, const char *chs, size_t len);

// Other library utilites
int stxuni8f32(char *dst, uint32_t wc);

#endif
