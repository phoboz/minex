// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "object.h"
#include "wrap.h"

// ---------------------------------------------------------------------------

typedef signed long wrap_t;

static wrap_t wrap(
	wrap_t kx,
	wrap_t kLowerBound,
	wrap_t kUpperBound
	)
{
	wrap_t range = kUpperBound - kLowerBound + 1;

	kx = ((kx-kLowerBound) % range);
	
	if (kx<0)
	{
		return kUpperBound + 1 + kx;
	}
	else
	{
		return kLowerBound + kx;
	}
}

unsigned int wrap_check(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	)
{
	wrap_t y, x;
	unsigned int result = 0;

	y = (wrap_t) p0[0] + (wrap_t) dy;
	x = (wrap_t) p0[1] + (wrap_t) dx;

	if (y >= OBJECT_WRAP_MIN_Y && y <= OBJECT_WRAP_MAX_Y)
	{
		p1[0] = (signed int) y;
	}
	else
	{
		result |= WRAP_Y;
	}

	if (x >= OBJECT_WRAP_MIN_X && x <= OBJECT_WRAP_MAX_X)
	{
		p1[1] = (signed int) x;
	}
	else
	{
		result |= WRAP_X;
	}

	return result;
}

unsigned int wrap_translate_y(
	signed int *y1,
	signed int y0,
	signed int dy
	)
{
	wrap_t y;
	unsigned int result = 0;

	y = (wrap_t) y0 + (wrap_t) dy;

	if (y >= OBJECT_WRAP_MIN_Y && y <= OBJECT_WRAP_MAX_Y)
	{
		*y1 = (signed int) y;
	}
	else
	{
		*y1 = (signed int) wrap(y, OBJECT_WRAP_MIN_Y, OBJECT_WRAP_MAX_Y);
		result |= WRAP_Y;
	}

	return result;
}

unsigned int wrap_translate_x(
	signed int *x1,
	signed int x0,
	signed int dx
	)
{
	wrap_t x;
	unsigned int result = 0;

	x = (wrap_t) x0 + (wrap_t) dx;

	if (x >= OBJECT_WRAP_MIN_X && x <= OBJECT_WRAP_MAX_X)
	{
		*x1 = (signed int) x;
	}
	else
	{
		*x1 = (signed int) wrap(x, OBJECT_WRAP_MIN_X, OBJECT_WRAP_MAX_X);
		result |= WRAP_Y;
	}

	return result;
}

unsigned int wrap_translate(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	)
{
	wrap_t y, x;
	unsigned int result = 0;

	y = (wrap_t) p0[0] + (wrap_t) dy;
	x = (wrap_t) p0[1] + (wrap_t) dx;

	if (y >= OBJECT_WRAP_MIN_Y && y <= OBJECT_WRAP_MAX_Y)
	{
		p1[0] = (signed int) y;
	}
	else
	{
		p1[0] = (signed int) wrap(y, OBJECT_WRAP_MIN_Y, OBJECT_WRAP_MAX_Y);
		result |= WRAP_Y;
	}

	if (x >= OBJECT_WRAP_MIN_X && x <= OBJECT_WRAP_MAX_X)
	{
		p1[1] = (signed int) x;
	}
	else
	{
		p1[1] = (signed int) wrap(x, OBJECT_WRAP_MIN_X, OBJECT_WRAP_MAX_X);
		result |= WRAP_X;
	}

	return result;
}

// ***************************************************************************
// end of file
// ***************************************************************************
