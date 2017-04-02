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

// Inline functions
static inline size_t
stxavail(stx *sp) {return sp->size ? sp->size - sp->len - 1 : 0;}

// Extern functions
stx *stxnew(stx *sp, size_t n);
stx *stxdel(stx *sp);

stx *stxgrow(stx *sp, size_t n);
stx *stxensure_size(stx *sp, size_t n);
stx *stxvalid(stx *sp);

bool stxeq(const stx *s1, const stx *s2);
void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t len);
stx *stxcpy_mem(stx *sp, const char *src, size_t len);
stx *stxcpy_str(stx *sp, const char *src);
stx *stxcpy_stx(stx *sp, const stx *src);

stx *stxins_mem(stx *sp, size_t pos, const char *src, size_t len);
stx *stxins_str(stx *sp, size_t pos, const char *src);
stx *stxins_uni(stx *sp, size_t pos, uint32_t wc);
stx *stxins_stx(stx *s1, size_t pos, const stx *s2);

stx *stxapp_mem(stx *sp, const char *src, size_t len);
stx *stxapp_str(stx *sp, const char *src);
stx *stxapp_uni(stx *sp, uint32_t wc);
stx *stxapp_stx(stx *s1, const stx *s2);

stx *stxslice(stx *slice, stx *sp, size_t begin, size_t end);
stx *stxfind_mem(stx *slice, stx *sp, const char *src, size_t len);
stx *stxfind_str(stx *slice, stx *sp, const char *src);
stx *stxfind_stx(stx *slice, stx *sp, const stx *src);

stx *stxrstrip(stx *sp, const char *chs, size_t len);
stx *stxlstrip(stx *sp, const char *chs, size_t len);
stx *stxstrip(stx *sp, const char *chs, size_t len);

int stxuni8f32(char *dst, uint32_t wc);

#endif
