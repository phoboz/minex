// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "ship.h"

// ---------------------------------------------------------------------------

struct object *ship_list = 0;
struct object *ship_free_list = 0;

static const signed char ship_front_vec[]=
{
	1,	0
};

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
	init_object(&ship->obj, y, x, h, w, &ship_list);
	ship->shape = shape;
	ship->scale = scale;

	ship->obj_pos[0]	= y;
	ship->obj_pos[1]	= x;

	ship->obj_angle		= obj_angle;
	ship->old_obj_angle	= obj_angle;
	Rot_VL_Mode(ship->obj_angle, (signed int *) shape, ship->obj_vlist);

	ship->world_angle	= world_angle;
	Rot_VL_ab(world_angle, 0, ship->obj.pos, ship->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) ship->obj_vlist, &ship->world_vlist);

	ship->speed = 0;
	Rot_VL_ab(obj_angle, 0, (signed int *) ship_front_vec, ship->front_vec);
}

void deinit_ship(
	struct ship *ship
	)
{
	deinit_object(&ship->obj, &ship_list);
	give_object(&ship->obj, &ship_free_list);
}

void move_ships(
	struct player *player
	)
{
	unsigned int update_view;

	struct ship *ship;
	struct ship *rem = 0;

	ship = (struct ship *) ship_list;
	while (ship != 0)
	{
		update_view = 0;
		if (ship->obj_angle != ship->old_obj_angle)
		{
			Rot_VL_Mode(ship->obj_angle, (signed int *) ship->shape, ship->obj_vlist);
			Rot_VL_ab(ship->obj_angle, 0, (signed int *) ship_front_vec, ship->front_vec);
			update_view = 1;
		}

		if (ship->speed)
		{
			ship->obj_pos[0] += ship->speed * ship->front_vec[0];
			ship->obj_pos[1] += ship->speed * ship->front_vec[1];
			update_view = 1;
		}

		if (update_view || player->update_view)
		{
			ship->world_angle = player->angle;
			ship->obj.pos[0] = ship->obj_pos[0] - player->obj.pos[0];
			ship->obj.pos[1] = ship->obj_pos[1] + player->obj.pos[1];

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

			dp_VIA_t1_cnt_lo = ship->scale;
			Draw_VLp(ship->world_vlist);
		}

		ship = (struct ship *) ship->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
