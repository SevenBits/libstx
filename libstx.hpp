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

	int ensure_size(stx *sp, size_t n)
	{return stxensure_size(sp, n);}

	int valid(stx *sp)
	{return stxvalid(sp);}

	bool eq(const stx *s1, const stx *s2)
	{return stxeq(s1, s2);}

	void swap(stx *s1, stx *s2)
	{return stxswap(s1, s2);}

	stx *trunc(stx *sp, size_t len)
	{return stxtrunc(sp, len);}

	stx *cpy(stx *sp, const char *src, size_t len)
	{return stxcpy_mem(sp, src, len);}

	stx *cpy(stx *sp, const char *src)
	{return stxcpy_str(sp, src);}

	stx *cpy(stx *sp, const stx *src)
	{return stxcpy_stx(sp, src);}

	stx *ins(stx *sp, size_t pos, const char *src, size_t len)
	{return stxins_mem(sp, pos, src, len);}

	stx *ins(stx *sp, size_t pos, const char *src)
	{return stxins_str(sp, pos, src);}

	stx *ins(stx *sp, size_t pos, uint32_t wc)
	{return stxins_uni(sp, pos, wc);}

	stx *ins(stx *sp, size_t pos, const stx *src)
	{return stxins_stx(sp, pos, src);}

	stx *app(stx *sp, const char *src, size_t len)
	{return stxapp_mem(sp, src, len);}

	stx *app(stx *sp, const char *src)
	{return stxapp_str(sp, src);}

	stx *app(stx *sp, uint32_t wc)
	{return stxapp_uni(sp, wc);}

	stx *app(stx *sp, const stx *src)
	{return stxapp_stx(sp, src);}

	stx *find(stx *slice, stx *sp, const char *src, size_t len)
	{return stxfind_mem(slice, sp, src, len);}

	stx *find(stx *slice, stx *sp, const char *src)
	{return stxfind_str(slice, sp, src);}

	stx *find(stx *slice, stx *sp, const stx *src)
	{return stxfind_stx(slice, sp, src);}

	stx *slice(stx *slice, stx *sp, size_t begin, size_t end)
	{return stxslice(slice, sp, begin, end);}

	stx *rstrip(stx *sp, const char *chs, size_t len)
	{return stxrstrip(sp, chs, len);}

	stx *lstrip(stx *sp, const char *chs, size_t len)
	{return stxlstrip(sp, chs, len);}

	stx *strip(stx *sp, const char *chs, size_t len)
	{return stxstrip(sp, chs, len);}

	int uni8f32(char *dst, uint32_t wc)
	{return stxuni8f32(dst, wc);}
}

#endif
