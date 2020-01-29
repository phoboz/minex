// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "ship.h"

// ---------------------------------------------------------------------------

struct object *ship_list = 0;
struct object *ship_free_list = 0;

void init_ship(
	struct ship *ship,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int obj_angle,
	unsigned int world_angle,
	unsigned int scale,
	const signed char *shape
	)
{
	take_object(&ship->obj, &ship_free_list);
	init_object(&ship->obj, y, x, h, w, scale, shape, &ship_list);

	ship->obj_angle		= obj_angle;
	ship->old_obj_angle	= obj_angle;
	Rot_VL_Mode(ship->obj_angle, (signed int *) ship->obj.shape, ship->obj_vlist);

	ship->world_angle		= world_angle;
	ship->old_world_angle	= world_angle;
	Rot_VL_ab(world_angle, 0, ship->obj.pos, ship->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) ship->obj_vlist, &ship->world_vlist);
}

void deinit_ship(
	struct ship *ship
	)
{
	deinit_object(&ship->obj, &ship_list);
	give_object(&ship->obj, &ship_free_list);
}

void move_ships(void)
{
	unsigned int prev_update;

	struct ship *ship;
	struct ship *rem = 0;

	ship = (struct ship *) ship_list;
	while (ship != 0)
	{
		prev_update = 0;
		//if (ship->obj_angle != ship->old_obj_angle)
		{
			Rot_VL_Mode(ship->obj_angle, (signed int *) ship->obj.shape, ship->obj_vlist);
			prev_update = 1;
		}

		//if (prev_update || ship->world_angle != ship->old_world_angle)
		{
			Rot_VL_ab(ship->world_angle, 0, ship->obj.pos, ship->obj.world_pos);
			Rot_VL_Mode(ship->world_angle, (signed int *) ship->obj_vlist, ship->world_vlist);
		}
	
		if (/*remove*/0)
		{
			rem = ship;
		}

		ship = (struct ship *) ship->obj.next;

		if (rem != 0)
		{
			deinit_ship(rem);
			rem = 0;
		}
	}
}

void draw_ships(void)
{
	struct ship *ship;

	ship = (struct ship *) ship_list;
	while (ship != 0)
	{
		if (ship->obj.pos[0] >= OBJECT_MIN_Y && ship->obj.pos[0] <= OBJECT_MAX_Y &&
		    ship->obj.pos[1] >= OBJECT_MIN_X && ship->obj.pos[1] <= OBJECT_MAX_X)
		{
			Reset0Ref();
			Moveto_d(0, 0);

			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Moveto_d(ship->obj.world_pos[0], ship->obj.world_pos[1]);
			dp_VIA_t1_cnt_lo = ship->obj.scale;
			Draw_VLp(ship->world_vlist);
		}

		ship = (struct ship *) ship->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
