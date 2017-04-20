/* See LICENSE file for copyright and license details */
#ifndef LIBSTX_H
#define LIBSTX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct stx {
    char *mem;
    size_t len;
    size_t size;
};

typedef struct stx stx;

int stxalloc(stx *sp, size_t n);
void stxdel(const stx *sp);

int stxgrow(stx *sp, size_t n);
int stxensuresize(stx *sp, size_t n);
int stxvalid(stx *sp);

size_t stxavail(stx *sp);
bool stxeq(const stx *s1, const stx *s2);
void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t len);
stx *stxterm(stx *sp);

stx *stxcpy_mem(stx *sp, const char *src, size_t len);
stx *stxcpy_str(stx *sp, const char *src);
stx *stxcpy_stx(stx *sp, const stx *src);

stx *stxins_mem(stx *sp, size_t pos, const char *src, size_t len);
stx *stxins_str(stx *sp, size_t pos, const char *src);
stx *stxins_uni(stx *sp, size_t pos, uint32_t wc);
stx *stxins_stx(stx *sp, size_t pos, const stx *src);

stx *stxapp_mem(stx *sp, const char *src, size_t len);
stx *stxapp_str(stx *sp, const char *src);
stx *stxapp_uni(stx *sp, uint32_t wc);
stx *stxapp_stx(stx *sp, const stx *src);
// Note, last argument must be 0 for the function to terminate properly.
stx *stxvapp_str(stx *sp, const char *args, ...);
stx *stxvapp_uni(stx *sp, uint32_t args, ...);
stx *stxvapp_stx(stx *sp, const stx *args, ...);

stx *stxfind_mem(stx *slice, stx *sp, const char *src, size_t len);
stx *stxfind_str(stx *slice, stx *sp, const char *src);
stx *stxfind_stx(stx *slice, stx *sp, const stx *src);
stx *stxslice(stx *slice, const stx *sp, size_t begin, size_t end);

stx *stxrstrip(stx *sp, const char *chs, size_t len);
stx *stxlstrip(stx *sp, const char *chs, size_t len);
stx *stxstrip(stx *sp, const char *chs, size_t len);

int stxuni8f32(char *dst, uint32_t wc);

#endif
