#ifndef _IMATH_H
#define _IMATH_H

#define abs(a)		(((a) < 0) ? (-a) : (a))
#define max(a, b)		((a) > (b) ? (a) : (b))
#define min(a, b)		((a) < (b) ? (a) : (b))

unsigned int isqrt8(
	unsigned int num
	);

unsigned long isqrt16(
	unsigned long value
	);

#endif
