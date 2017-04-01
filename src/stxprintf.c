// See LICENSE file for copyright and license details

#include <stdarg.h>
#include <stdio.h>

#include "internal.h"

stx *
stxfmt(stx *s1, const char *fmt, ...)
{
	int err;
	int len;
	va_list ap;

	va_start(ap, fmt);
	if (0 > (len = vsnprintf(s1->mem, s1->size, fmt, ap)))
		return NULL;
	va_end(ap);
	stxterm(s1, len);

	return s1;
}
