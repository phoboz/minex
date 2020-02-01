// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "object.h"

// ---------------------------------------------------------------------------

void  give_object(
	struct object *obj,
	struct object **head
	)
{
	obj->active = 0;

	// Add object to list
	if (head)
	{
		obj->prev = 0;
		obj->next = *head;
		*head = obj;

		if (obj->next != 0)
		{
			obj->next->prev = obj;
		}
	}
	else
	{
		obj->prev = 0;
		obj->next = 0;
	}
}

void take_object(
	struct object *obj,
	struct object **head
	)
{
	// Remove object from list
	if (head)
	{
		if (obj->prev != 0)
		{
			obj->prev->next = obj->next;
		}

		if (obj->next != 0)
		{
			obj->next->prev = obj->prev;
		}

		if (*head == obj)
		{
			*head = obj->next;
		}
	}
}

void init_object(
	struct object *obj,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	struct object **head
	)
{
	obj->active = 1;

	obj->center_pos[0] = 0;
	obj->center_pos[1] = 0;

	obj->rel_pos[0] = y;
	obj->rel_pos[1] = x;

	obj->world_pos[0] = y;
	obj->world_pos[1] = x;

	obj->dim_2[0] = -(h >> 1);
	obj->dim_2[1] = w >> 1;

	// Add object to list
	if (head)
	{
		obj->prev = 0;
		obj->next = *head;
		*head = obj;

		if (obj->next != 0)
		{
			obj->next->prev = obj;
		}
	}
	else
	{
		obj->prev = 0;
		obj->next = 0;
	}
}

void deinit_object(
	struct object *obj,
	struct object **head
	)
{
	if (obj->active)
	{
		// Remove object from list
		if (head)
		{
			if (obj->prev != 0)
			{
				obj->prev->next = obj->next;
			}

			if (obj->next != 0)
			{
				obj->next->prev = obj->prev;
			}

			if (*head == obj)
			{
				*head = obj->next;
			}
		}
	}

	obj->active = 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
