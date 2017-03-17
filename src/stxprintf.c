// See LICENSE file for copyright and license details

#include <stdarg.h>
#include <stdio.h>

#include "internal.h"

int
stxsnprintf(stx *s1, size_t size, const char *fmt, ...)
{
	int err;
	int len;
	va_list ap;

	if (size_add_overflows(size, 1))
		return ESTX_OVERFLOW;

	err = stxexp(s1, size + 1);
	if (err)
		return err;

	va_start(ap, fmt);
	if (0 > (len = vsnprintf(s1->mem, size, fmt, ap)))
		return -1;
	va_end(ap);
	stxterm(s1, len);

	return s1->len;
}
