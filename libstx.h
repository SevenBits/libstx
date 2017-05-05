/* See LICENSE file for copyright and license details */
#ifndef LIBSTX_H
#define LIBSTX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __STDC__
#if (__STDC_VERSION__ == 201112L)
#define stxcpy(sp, src) _Generic((src), \
		char *: stxcpy_str \
		spx: stxcpy_spx)(sp, src)

#define stxins(sp, src) _Generic((src), \
		char *: stxins_str \
		uint32_t: stxins_uni \
		spx: stxins_spx)(sp, src)

#define stxapp(sp, src) _Generic((src), \
		char *: stxapp_str \
		uint32_t: stxapp_uni \
		spx: stxapp_spx)(sp, src)

#define stxfind(sp, src) _Generic((src), \
		char *: stxfind_str \
		spx: stxfind_spx)(sp, src)
#endif
#endif

struct stx {
	char *mem;
	size_t len;
	size_t size;
};

struct spx {
	char *mem;
	size_t len;
};

typedef struct stx stx;
typedef struct spx spx;

int stxalloc(stx *sp, size_t n);
void stxdel(const stx *sp);

int stxgrow(stx *sp, size_t n);
int stxensuresize(stx *sp, size_t n);
int stxvalid(stx *sp);

size_t stxavail(stx *sp);

bool stxeq(spx s1, spx s2);

void stxswap(stx *s1, stx *s2);
stx *stxtrunc(stx *sp, size_t len);
stx *stxterm(stx *sp);
spx stxref(const stx *sp);

stx *stxcpy_mem(stx *sp, const char *src, size_t len);
stx *stxcpy_str(stx *sp, const char *src);
stx *stxcpy_spx(stx *sp, const spx src);

stx *stxins_mem(stx *sp, size_t pos, const char *src, size_t len);
stx *stxins_str(stx *sp, size_t pos, const char *src);
stx *stxins_uni(stx *sp, size_t pos, uint32_t wc);
stx *stxins_spx(stx *sp, size_t pos, const spx src);

stx *stxapp_mem(stx *sp, const char *src, size_t len);
stx *stxapp_str(stx *sp, const char *src);
stx *stxapp_uni(stx *sp, uint32_t wc);
stx *stxapp_spx(stx *sp, const spx src);

spx stxfind_mem(const spx haystack, const char *needle, size_t len);
spx stxfind_str(const spx haystack, const char *needle);
spx stxfind_spx(const spx haystack, const spx needle);

spx stxslice(const spx sp, size_t begin, size_t end);

stx *stxrstrip(stx *sp, const char *chs, size_t len);
stx *stxlstrip(stx *sp, const char *chs, size_t len);
stx *stxstrip(stx *sp, const char *chs, size_t len);

int stxuni8f32(char *dst, uint32_t wc);

#endif
