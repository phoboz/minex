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
	unsigned int scale,
	const signed char *shape,
	struct object **head
	)
{
	obj->active = 1;

	obj->y = y;
	obj->x = x;
	obj->h = h;
	obj->w = w;
	obj->h_2 = h >> 1;
	obj->w_2 = w >> 1;

	obj->scale = scale;
	obj->shape = shape;

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

void rotate_position_object(
	struct object *obj,
	unsigned int angle,
	signed char *dest
	)
{
	static signed char v[2];

	v[0] = obj->y;
	v[1] = obj->x;

	Rot_VL_ab(angle, 0, v, dest);
}

void translate_object(
	struct object *obj,
	signed char *dest
	)
{
	const signed char *p_src = obj->shape;
	signed char *p_dest = dest;

	while (1)
	{
		*p_dest = *p_src;
		if (*p_src == 0x01)
		{
			break;
		}

		p_dest++;
		p_src++;

		*p_dest = (*p_src) + obj->y;
		p_dest++;
		p_src++;

		*p_dest = (*p_src) + obj->x;
		p_dest++;
		p_src++;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
