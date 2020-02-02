// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "ship.h"

// ---------------------------------------------------------------------------

unsigned int num_ships = 0;
struct ship ships[MAX_SHIPS];

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
	init_object(&ship->obj, y, x, h, w);
	ship->shape = shape;
	ship->scale = scale;

	ship->obj_pos[0]	= y;
	ship->obj_pos[1]	= x;

	ship->rel_pos[0] = y;
	ship->rel_pos[1] = x;

	ship->obj_angle		= obj_angle;
	ship->old_obj_angle	= obj_angle;
	Rot_VL_Mode(ship->obj_angle, (signed int *) shape, ship->obj_vlist);

	ship->world_angle	= world_angle;
	Rot_VL_ab(world_angle, 0, ship->obj.dim_2, ship->obj.center_pos);
	Rot_VL_ab(world_angle, 0, ship->rel_pos, ship->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) ship->obj_vlist, &ship->world_vlist);

	ship->speed = 0;
	Rot_VL_ab(obj_angle, 0, (signed int *) ship_front_vec, ship->front_vec);

	num_ships++;
}

void deinit_ship(
	struct ship *ship
	)
{
	deinit_object(&ship->obj);
	num_ships--;
}

void move_ships(
	struct player *player
	)
{
	unsigned int i;
	unsigned int update_view;
	struct ship *ship;
	struct bullet *bullet;

	for (i = 0; i < MAX_SHIPS; i++)
	{
		ship = &ships[i];
		if (ship->obj.active)
		{
			update_view = 0;
			if (ship->obj_angle != ship->old_obj_angle)
			{
				// TODO: Rotate ship center vector
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
				ship->rel_pos[0] = ship->obj_pos[0] - player->rel_pos[0];
				ship->rel_pos[1] = ship->obj_pos[1] + player->rel_pos[1];

				Rot_VL_ab(ship->world_angle, 0, ship->obj.dim_2, ship->obj.center_pos);
				Rot_VL_ab(ship->world_angle, 0, ship->rel_pos, ship->obj.world_pos);
				Rot_VL_Mode(ship->world_angle, (signed int *) ship->obj_vlist, ship->world_vlist);
			}
	
			if (1)
			{
				unsigned int j;
				for (j = 0; j < MAX_BULLETS; j++)
				{
					bullet = &bullets[j];
					if (bullet->active)
					{
						if (hit_object_bullet(bullet, &ship->obj))
						{
							//mine->state = MINE_STATE_REMOVE;
							deinit_bullet(bullet);
						}
					}
				}
			}
		}
	}
}

void draw_ships(void)
{
	unsigned int i;
	struct ship *ship;
	signed int center_y, center_x;

	for (i = 0; i < MAX_SHIPS; i++)
	{
		ship = &ships[i];
		if (ship->obj.active)
		{
			center_y = ship->obj.world_pos[0] + ship->obj.center_pos[0];
			center_x = ship->obj.world_pos[1] + ship->obj.center_pos[1];
			if (center_y >= OBJECT_MIN_Y && center_y <= OBJECT_MAX_Y &&
				center_y >= OBJECT_MIN_X && center_y <= OBJECT_MAX_X)
			{
				Reset0Ref();
				Moveto_d(0, 0);
				dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
				Moveto_d(ship->obj.world_pos[0], ship->obj.world_pos[1]);

//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
				Moveto_d(ship->obj.center_pos[0], ship->obj.center_pos[1]);

				Moveto_d(-ship->obj.dim_2[0], -ship->obj.dim_2[1]);
				Draw_Line_d(0, ship->obj.dim_2[1] << 1);
				Draw_Line_d(ship->obj.dim_2[0] << 1, 0);
				Draw_Line_d(0, -ship->obj.dim_2[1] << 1);
				Draw_Line_d(-ship->obj.dim_2[0] << 1, 0);
				Moveto_d(ship->obj.dim_2[0], ship->obj.dim_2[1]);

				Moveto_d(-ship->obj.center_pos[0], -ship->obj.center_pos[1]);
#endif

				dp_VIA_t1_cnt_lo = ship->scale;
				Draw_VLp(ship->world_vlist);
			}
		}
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
