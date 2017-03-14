// See LICENSE file for copyright and license details

#include <stdarg.h>
#include <stdio.h>

#include "internal.h"

int oystr_snprintf(struct oystr *s1, size_t size, const char *fmt, ...)
{
	int len;
	va_list ap;

	if (0 > oystr_ensure_size(s1, size))
		return OYSTR_ERR;

	va_start(ap, fmt);
	if (0 > (len = vsnprintf(s1->buf, size, fmt, ap)))
		return -1;
	va_end(ap);
	oystr_setlen(s1, len);

	return s1->len;
}
