/* See LICENSE file for copyright and license details */
#ifndef STX_H
#define STX_H

struct stx {
    size_t len;
    size_t size;
    char *mem;
};

typedef struct stx stx;

stx *stxalloc(stx *sp, size_t n);
stx *stxdel(stx *sp);

stx *stxgrow(stx *sp, size_t n);
stx *stxensure_size(stx *sp, size_t n);
stx *stxvalid(stx *sp);

size_t stxavail(stx *sp);
bool stxeq(const stx *s1, const stx *s2);
void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t len);
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

stx *stxfind_mem(stx *slice, stx *sp, const char *src, size_t len);
stx *stxfind_str(stx *slice, stx *sp, const char *src);
stx *stxfind_stx(stx *slice, stx *sp, const stx *src);
stx *stxslice(stx *slice, stx *sp, size_t begin, size_t end);

stx *stxrstrip(stx *sp, const char *chs, size_t len);
stx *stxlstrip(stx *sp, const char *chs, size_t len);
stx *stxstrip(stx *sp, const char *chs, size_t len);

int stxuni8f32(char *dst, uint32_t wc);

#endif
