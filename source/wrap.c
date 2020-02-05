// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "object.h"

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

void wrap_translate(
	signed int p1[2],
	signed int p0[2],
	signed int dy,
	signed int dx
	)
{
	wrap_t y, x;

	y = (wrap_t) p0[0] + (wrap_t) dy;
	x = (wrap_t) p0[1] + (wrap_t) dx;

	if (y >= OBJECT_MIN_Y && y <= OBJECT_MAX_Y)
	{
		p1[0] = (signed int) y;
	}
	else
	{
		p1[0] = (signed int) wrap(y, OBJECT_WRAP_MIN_Y, OBJECT_WRAP_MAX_Y);
	}

	if (x >= OBJECT_MIN_X && x <= OBJECT_MAX_X)
	{
		p1[1] = (signed int) x;
	}
	else
	{
		p1[1] = (signed int) wrap(x, OBJECT_WRAP_MIN_X, OBJECT_WRAP_MAX_X);
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
