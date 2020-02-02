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

// ***************************************************************************
// end of file
// ***************************************************************************
