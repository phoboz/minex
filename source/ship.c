// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "wrap.h"
#include "ship_data.h"
#include "ship.h"

// ---------------------------------------------------------------------------

struct element *ship_list = 0;
struct element *ship_free_list = 0;

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
	unsigned int num_hits,
	struct player *player,
	unsigned int scale,
	const signed char *shape
	)
{
	take_element(&ship->obj.elmnt, &ship_free_list);
	init_object(&ship->obj, y, x, h, w, &ship_list);
	ship->shape = shape;
	ship->scale = scale;

	ship->obj_pos[0]	= y;
	ship->obj_pos[1]	= x;

	ship->rel_pos[0] = y;
	ship->rel_pos[1] = x;

	ship->state_counter	= 0;
	ship->state			= SHIP_STATE_ACTIVATE;
	ship->num_hits		= num_hits;

	ship->obj_angle		= obj_angle;
	ship->old_obj_angle	= obj_angle;
	Rot_VL_Mode(ship->obj_angle, (signed int *) shape, ship->obj_vlist);

	ship->world_angle = player->angle;
	wrap_translate(ship->rel_pos, ship->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
	Rot_VL_ab(player->angle, 0, ship->rel_pos, ship->obj.world_pos);
	Rot_VL_Mode(player->angle, ship->obj_vlist, &ship->world_vlist);

	ship->speed = 0;
	Rot_VL_ab(obj_angle, 0, (signed int *) ship_front_vec, ship->front_vec);
}

void deinit_ship(
	struct ship *ship
	)
{
	deinit_object(&ship->obj, &ship_list);
	give_element(&ship->obj.elmnt, &ship_free_list);
}

unsigned int move_ships(
	struct player *player
	)
{
	unsigned int status = 0;

	unsigned int update_view;

	struct ship *ship;
	struct ship *rem_ship = 0;

	struct bullet *bullet;
	struct bullet *rem_bullet = 0;

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

		if (ship->state == SHIP_STATE_ACTIVE)
		{
			if (ship->speed)
			{
				wrap_translate(
					ship->obj_pos,
					ship->obj_pos,
					ship->speed * ship->front_vec[0],
					ship->speed * ship->front_vec[1]
					);
				update_view = 1;
			}
		}

		if (update_view || player->update_view)
		{
			ship->world_angle = player->angle;
			wrap_translate(ship->rel_pos, ship->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
			Rot_VL_ab(ship->world_angle, 0, ship->rel_pos, ship->obj.world_pos);
			Rot_VL_Mode(ship->world_angle, ship->obj_vlist, ship->world_vlist);
		}

		if (ship->state == SHIP_STATE_ACTIVATE)
		{
			if (++ship->state_counter == SHIP_ACTIVATE_TRESHOLD)
			{
				ship->state_counter = 0;
				ship->state = SHIP_STATE_ACTIVE;
			}
		}
		else if (ship->state == SHIP_STATE_ACTIVE)
		{
			bullet = (struct bullet *) bullet_list;
			while (bullet)
			{
				if (hit_object_bullet(bullet, &ship->obj))
				{
					if (--ship->num_hits <= 0)
					{
						ship->state = SHIP_STATE_EXPLODE;
						ship->speed = 0;
						ship->state_counter = 0;
						status |= SHIP_STATUS_EXPLODE;
					}
					else
					{
						status |= SHIP_STATUS_HIT;
					}

					rem_bullet = bullet;
				}

				bullet = (struct bullet *) bullet->elmnt.next;

				if (rem_bullet != 0)
				{
					deinit_bullet(rem_bullet);
					rem_bullet = 0;
				}
			}

#ifndef SHIP_NO_HIT
			if (player->anim.obj.active)
			{
				if (check_box_object(
					&ship->obj,
					player->anim.obj.dim_2[0],
					-player->anim.obj.dim_2[1],
					-player->anim.obj.dim_2[0],
					player->anim.obj.dim_2[1]
					))
				{
					hit_player(player);
					status |= SHIP_STATUS_HIT_PLAYER;
				}
			}
#endif
		}
		else if (ship->state == SHIP_STATE_EXPLODE)
		{
			if (++ship->state_counter >= SHIP_EXPLODE_TRESHOLD)
			{
				ship->state_counter = 0;
				ship->state = SHIP_STATE_REMOVE;
			}
		}
		else if (ship->state == SHIP_STATE_REMOVE)
		{
			rem_ship = ship;
		}
	
		ship = (struct ship *) ship->obj.elmnt.next;

		if (rem_ship != 0)
		{
			deinit_ship(rem_ship);
			rem_ship = 0;
		}
	}

	return status;
}

void draw_ships(void)
{
	struct ship *ship;
	signed int y, x;
	signed int h, w;

	ship = (struct ship *) ship_list;
	while (ship != 0)
	{
		y = ship->obj.world_pos[0];
		x = ship->obj.world_pos[1];
		h = -ship->obj.dim_2[0];
		w = ship->obj.dim_2[1];
		if (!(y + h < OBJECT_MIN_Y || y - h > OBJECT_MAX_Y ||
			x + w < OBJECT_MIN_X || x - w > OBJECT_MAX_X))
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Moveto_d(y, x);

//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
			Moveto_d(-ship->obj.dim_2[0], -ship->obj.dim_2[1]);

			Draw_Line_d(0, ship->obj.dim_2[1] << 1);
			Draw_Line_d(ship->obj.dim_2[0] << 1, 0);
			Draw_Line_d(0, -ship->obj.dim_2[1] << 1);
			Draw_Line_d(-ship->obj.dim_2[0] << 1, 0);

			Moveto_d(ship->obj.dim_2[0], ship->obj.dim_2[1]);
#endif

			if (ship->state == SHIP_STATE_IDLE)
			{
				Dot_here();
			}
			else if (ship->state == SHIP_STATE_ACTIVATE)
			{
				Intensity_3F();
				dp_VIA_t1_cnt_lo = (SHIP_DRAW_SCALE - SHIP_ACTIVATE_TRESHOLD) + ship->state_counter;
				Draw_VLp(ship->world_vlist);
				Intensity_7F();
			}
			else if (ship->state == SHIP_STATE_ACTIVE)
			{
				dp_VIA_t1_cnt_lo = ship->scale;
				Draw_VLp(ship->world_vlist);
			}
			else if (ship->state == SHIP_STATE_EXPLODE)
			{
				dp_VIA_t1_cnt_lo = 0x10 + (ship->state_counter << 2);
				Draw_VLp((signed char *) ship_explode);
			}
		}

		ship = (struct ship *) ship->obj.elmnt.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
