// See LICENSE file for copyright and license details

#include <stdarg.h>
#include <stdio.h>

#include "internal.h"

stx *
stxsnprintf(stx *s1, size_t size, const char *fmt, ...)
{
	int err;
	int len;
	va_list ap;

	if (!stx_ensure_size(s1, size))
		return NULL;

	va_start(ap, fmt);
	if (0 > (len = vsnprintf(s1->mem, size, fmt, ap)))
		return NULL;
	va_end(ap);
	stxterm(s1, len);

	return s1;
}
