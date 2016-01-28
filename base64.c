/*
 * Copyright 2016 jayay
 * Copyright 2008-2013 Various Authors
 * Copyright 2004 Timo Hirvonen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "base64.h"
#include "xmalloc.h"

char *base64_encode(const char *str)
{
	static const char t[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int str_len, buf_len, i, s, d;
	char *buf;
	unsigned char b0, b1, b2;

	str_len = strlen(str);
	buf_len = (str_len + 2) / 3 * 4 + 1;
	buf = xnew(char, buf_len);
	s = 0;
	d = 0;
	for (i = 0; i < str_len / 3; i++) {
		b0 = str[s++];
		b1 = str[s++];
		b2 = str[s++];

		/* 6 ms bits of b0 */
		buf[d++] = t[b0 >> 2];

		/* 2 ls bits of b0 . 4 ms bits of b1 */
		buf[d++] = t[((b0 << 4) | (b1 >> 4)) & 0x3f];

		/* 4 ls bits of b1 . 2 ms bits of b2 */
		buf[d++] = t[((b1 << 2) | (b2 >> 6)) & 0x3f];

		/* 6 ls bits of b2 */
		buf[d++] = t[b2 & 0x3f];
	}
	switch (str_len % 3) {
	case 2:
		b0 = str[s++];
		b1 = str[s++];

		/* 6 ms bits of b0 */
		buf[d++] = t[b0 >> 2];

		/* 2 ls bits of b0 . 4 ms bits of b1 */
		buf[d++] = t[((b0 << 4) | (b1 >> 4)) & 0x3f];

		/* 4 ls bits of b1 */
		buf[d++] = t[(b1 << 2) & 0x3f];

		buf[d++] = '=';
		break;
	case 1:
		b0 = str[s++];

		/* 6 ms bits of b0 */
		buf[d++] = t[b0 >> 2];

		/* 2 ls bits of b0 */
		buf[d++] = t[(b0 << 4) & 0x3f];

		buf[d++] = '=';
		buf[d++] = '=';
		break;
	case 0:
		break;
	}
	buf[d++] = 0;
	return buf;
}
