// See LICENSE file for copyright and license details
#include "internal.h"

enum {
	UTF8_H1 = 0x00,      // 1-byte header 0xxxxxxx.
	UTF8_H2 = 0x06 << 5, // 2-byte header 110xxxxx.
	UTF8_H3 = 0x0e << 4, // 3-byte header 1110xxxx.
	UTF8_H4 = 0x1e << 3, // 4-byte header 11110xxx.
	UTF8_HC = 0x02 << 6, // Continuation byte header 10xxxxxx.
};

int
stxuni8f32(char *dst, uint32_t wc)
{
    int len;
    uint32_t header;
    if (wc < 0x000080) {
        header = UTF8_H1;
        len = 1;
    } else if (wc < 0x000800) {
        header = UTF8_H2;
        len = 2;
    } else if (wc < 0x010000) {
        header = UTF8_H3;
        len = 3;
    } else if (wc < 0x200000) {
        header = UTF8_H4;
        len = 4;
    } else {
        // Invalid unicode character
        return 0;
    }

    int i;
    for (i = len-1; i > 0; --i) {
        dst[i] = (UTF8_HC | (wc & 0x3F));
        wc >>= 6;
    }
    dst[0] = (header | wc);

    return len;
}
