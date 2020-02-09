#ifndef _WRAP_H
#define _WRAP_H

#define WRAP_Y		0x01
#define WRAP_X		0x02

unsigned int wrap_check(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	);

unsigned int wrap_translate_y(
	signed int *y1,
	signed int y0,
	signed int dy
	);

unsigned int wrap_translate_x(
	signed int *x1,
	signed int x0,
	signed int dx
	);

unsigned int wrap_translate(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	);

#endif
