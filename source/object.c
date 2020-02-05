// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "object.h"

// ---------------------------------------------------------------------------

void init_object(
	struct object *obj,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	struct element **head
	)
{
	obj->active = 1;

	obj->center_pos[0] = 0;
	obj->center_pos[1] = 0;

	obj->world_pos[0] = y;
	obj->world_pos[1] = x;

	obj->dim_2[0] = -(h >> 1);
	obj->dim_2[1] = w >> 1;

	give_element(&obj->elmnt, head);
}

void deinit_object(
	struct object *obj,
	struct element **head
	)
{
	if (obj->active)
	{
		take_element(&obj->elmnt, head);
	}

	obj->active = 0;
}

#if 0
static signed int check_overflow(
	signed int a,
	signed int b
	)
{
	signed int result = 0;
	signed int sum = a + b;

	// out-of-range only possible when the signs are the same.
	if ((a < 0) == (b < 0))
	{
		if (a < 0)
		{
			// underflow here means the result is excessively negative.
			if (sum > b) result = -1; // underflow detected
		}
		else
		{
			if (sum < b) result = 1; // overflow detected
		}
	}

	return result;
}
static int check_overflow(
	int a,
	int b
	)
{
	int result = 0;

	if (a > 0 && b > 127 - a)
	{
		/* handle overflow */
		result = 1;
	}
	else if (a < 0 && b < -127 - a)
	{
		/* handle underflow */
		result = -1;
	}

    return result;
}
#endif

// ***************************************************************************
// end of file
// ***************************************************************************
