/* See LICENSE file for copyright and license details */
#ifndef LIBSTX_HPP
#define LIBSTX_HPP

/**
 * This header is used to wrap the libstx library with a C++ namespace, and to
 * change functions that had postfix types in their name to be overloaded
 * functions.
 */
namespace libstx {
	extern "C" {
	#include "libstx.h"
	}

	int alloc(stx *sp, size_t n)
	{return stxalloc(sp, n);}

	void del(stx *sp)
	{return stxdel(sp);}

	int grow(stx *sp, size_t n)
	{return stxgrow(sp, n);}

	int ensuresize(stx *sp, size_t n)
	{return stxensuresize(sp, n);}

	int valid(stx *sp)
	{return stxvalid(sp);}

	bool eq(const spx s1, const spx s2)
	{return stxeq(s1, s2);}

	void swap(stx *s1, stx *s2)
	{return stxswap(s1, s2);}

	stx *trunc(stx *sp, size_t n)
	{return stxtrunc(sp, n);}

	spx ref(const stx *sp)
	{return stxref(sp);}

	int dup(stx *sp, const char *src, size_t n)
	{return stxdup_mem(sp, src, n);}
	int dup(stx *sp, const char *src)
	{return stxdup_str(sp, src);}
	int dup(stx *sp, const spx src)
	{return stxdup_spx(sp, src);}

	stx *cpy(stx *sp, const char *src, size_t n)
	{return stxcpy_mem(sp, src, n);}
	stx *cpy(stx *sp, const char *src)
	{return stxcpy_str(sp, src);}
	stx *cpy(stx *sp, const spx src)
	{return stxcpy_spx(sp, src);}

	stx *ins(stx *sp, size_t pos, const char *src, size_t n)
	{return stxins_mem(sp, pos, src, n);}
	stx *ins(stx *sp, size_t pos, const char *src)
	{return stxins_str(sp, pos, src);}
	stx *ins(stx *sp, size_t pos, uint32_t wc)
	{return stxins_uni(sp, pos, wc);}
	stx *ins(stx *sp, size_t pos, const spx src)
	{return stxins_spx(sp, pos, src);}

	stx *app(stx *sp, const char *src, size_t n)
	{return stxapp_mem(sp, src, n);}
	stx *app(stx *sp, const char *src)
	{return stxapp_str(sp, src);}
	stx *app(stx *sp, uint32_t wc)
	{return stxapp_uni(sp, wc);}
	stx *app(stx *sp, const spx src)
	{return stxapp_spx(sp, src);}

	spx find(spx haystack, const char *needle, size_t n)
	{return stxfind_mem(haystack, needle, n);}
	spx find(spx haystack, const char *needle)
	{return stxfind_str(haystack, needle);}
	spx find(spx haystack, const spx needle)
	{return stxfind_spx(haystack, needle);}

	spx slice(const spx sp, size_t begin, size_t end)
	{return stxslice(sp, begin, end);}

	stx *rstrip(stx *sp, const char *chs, size_t n)
	{return stxrstrip(sp, chs, n);}
	stx *lstrip(stx *sp, const char *chs, size_t n)
	{return stxlstrip(sp, chs, n);}
	stx *strip(stx *sp, const char *chs, size_t n)
	{return stxstrip(sp, chs, n);}

	int uni8f32(char *dst, uint32_t wc)
	{return stxuni8f32(dst, wc);}
}

#endif
