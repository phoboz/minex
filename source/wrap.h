#ifndef _WRAP_H
#define _WRAP_H

#define WRAP_Y		0x01
#define WRAP_X		0x02

unsigned int wrap_translate(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	);

#endif
