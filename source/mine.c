// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "random.h"
#include "mine.h"

// ---------------------------------------------------------------------------

struct element *mine_list = 0;
struct element *mine_free_list = 0;

#define BLOW_UP 24

const signed char mine_explode[]=
{	(signed char) 0x00, -0x03*BLOW_UP, -0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, -0x01*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};

void init_mine(
	struct mine *mine,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int type,
	unsigned int treshold,
	struct player *player,
	unsigned int scale,
	const signed char *shape
	)
{
	take_element(&mine->obj.elmnt, &mine_free_list);
	init_object(&mine->obj, y, x, h, w, &mine_list);
	mine->scale = scale;
	mine->shape = shape;

	mine->obj_pos[0]	= y;
	mine->obj_pos[1]	= x;

	mine->rel_pos[0] = y;
	mine->rel_pos[1] = x;

	mine->velocity[0]	= 0;
	mine->velocity[1]	= 0;

	mine->type		= type;
	mine->state		= MINE_STATE_IDLE;
	mine->lo_counter	= 0;
	mine->hi_counter	= 0;
	mine->treshold	= treshold;

	mine->world_angle = player->angle;
	mine->rel_pos[0] = mine->obj_pos[0] - player->rel_pos[0];
	mine->rel_pos[1] = mine->obj_pos[1] + player->rel_pos[1];
	Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
	Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);
	Rot_VL_Mode(player->angle, (signed int*) shape, &mine->world_vlist);
}

void deinit_mine(
	struct mine *mine
	)
{
	deinit_object(&mine->obj, &mine_list);
	give_element(&mine->obj.elmnt, &mine_free_list);
}

void move_mines(
	struct player *player
	)
{
	unsigned int mine_index = 0;
	unsigned int update_view;

	struct mine *mine;
	struct mine *rem_mine = 0;

	struct bullet *bullet;
	struct bullet *rem_bullet = 0;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		update_view = 0;

		mine->lo_counter++;
		if (mine->lo_counter == 1)
		{
			if (mine_index == 0 || mine_index == 3 || mine_index == 6 || mine_index == 9)
			{
				if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
				{
					mine->obj_pos[0] += mine->velocity[0];
					mine->obj_pos[1] += mine->velocity[1];
					update_view = 1;
				}

				if (update_view || player->update_view)
				{
					mine->rel_pos[0] = mine->obj_pos[0] - player->rel_pos[0];
					mine->rel_pos[1] = mine->obj_pos[1] + player->rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
					}

					mine->world_angle = player->angle;
				}
			}
		}
		else if (mine->lo_counter == 2)
		{
			if (mine_index == 1 || mine_index == 4 || mine_index == 7 || mine_index == 10)
			{
				if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
				{
					mine->obj_pos[0] += mine->velocity[0];
					mine->obj_pos[1] += mine->velocity[1];
					update_view = 1;
				}

				if (update_view || player->update_view)
				{
					mine->rel_pos[0] = mine->obj_pos[0] - player->rel_pos[0];
					mine->rel_pos[1] = mine->obj_pos[1] + player->rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
					}

					mine->world_angle = player->angle;
				}
			}
		}
		else if (mine->lo_counter == 3)
		{
			mine->lo_counter = 0;

			if (mine->state == MINE_STATE_IDLE)
			{
				if (++mine->hi_counter == mine->treshold)
				{
					mine->hi_counter = 0;

					if (mine->state == MINE_STATE_IDLE)
					{
						mine->state = MINE_STATE_ACTIVE;
						if (mine->type == MINE_TYPE_DIRECTIONAL)
						{
							mine->velocity[0] = 6 - (signed int) (random() % 12U);
							mine->velocity[1] = 6 - (signed int) (random() % 12U);

							if (mine->velocity[0] == 0 && mine->velocity[1] == 0)
							{
								mine->velocity[0]++;
								mine->velocity[1]--;
							}
						}
						update_view = 1;
					}
				}
			}
			else if (mine->state == MINE_STATE_EXPLODE)
			{
				if (++mine->hi_counter == MINE_EXPLODE_TRESHOLD)
				{
					mine->hi_counter = 0;
					mine->state = MINE_STATE_REMOVE;
				}
			}

			if (mine_index == 2 || mine_index == 5 || mine_index == 8 || mine_index == 11)
			{
				if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
				{
					mine->obj_pos[0] += mine->velocity[0];
					mine->obj_pos[1] += mine->velocity[1];
					update_view = 1;
				}

				if (update_view || player->update_view)
				{
					mine->rel_pos[0] = mine->obj_pos[0] - player->rel_pos[0];
					mine->rel_pos[1] = mine->obj_pos[1] + player->rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
					}

					mine->world_angle = player->angle;
				}
			}
		}

		if (mine->state == MINE_STATE_ACTIVE)
		{
			bullet = (struct bullet *) bullet_list;
			while (bullet)
			{
				if (hit_object_bullet(bullet, &mine->obj))
				{
					mine->state = MINE_STATE_EXPLODE;
					mine->velocity[0] = mine->velocity[1] = 0;
					rem_bullet = bullet;
				}

				bullet = (struct bullet *) bullet->elmnt.next;

				if (rem_bullet != 0)
				{
					deinit_bullet(rem_bullet);
					rem_bullet = 0;
				}
			}
		}
		else if (mine->state == MINE_STATE_REMOVE)
		{
			rem_mine = mine;
		}

		mine = (struct mine *) mine->obj.elmnt.next;
		mine_index++;

		if (rem_mine != 0)
		{
			deinit_mine(rem_mine);
			rem_mine = 0;
		}
	}
}

void draw_mines(void)
{
	struct mine *mine;
	signed int center_y, center_x;
	signed int h, w;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		center_y = mine->obj.world_pos[0] + mine->obj.center_pos[0];
		center_x = mine->obj.world_pos[1] + mine->obj.center_pos[1];
		h = mine->obj.dim_2[0] << 1;
		w = mine->obj.dim_2[1] << 1;
		if (center_y - h >= OBJECT_MIN_Y && center_y + h <= OBJECT_MAX_Y &&
		    center_x + w >= OBJECT_MIN_X && center_x - w <= OBJECT_MAX_X)
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Moveto_d(mine->obj.world_pos[0], mine->obj.world_pos[1]);

			if (mine->state == MINE_STATE_IDLE)
			{
				Dot_here();
			}
			else if (mine->state == MINE_STATE_ACTIVE)
			{
//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
				Moveto_d(mine->obj.center_pos[0], mine->obj.center_pos[1]);

				Moveto_d(-mine->obj.dim_2[0], -mine->obj.dim_2[1]);
				Draw_Line_d(0, mine->obj.dim_2[1] << 1);
				Draw_Line_d(mine->obj.dim_2[0] << 1, 0);
				Draw_Line_d(0, -mine->obj.dim_2[1] << 1);
				Draw_Line_d(-mine->obj.dim_2[0] << 1, 0);
				Moveto_d(mine->obj.dim_2[0], mine->obj.dim_2[1]);

				Moveto_d(-mine->obj.center_pos[0], -mine->obj.center_pos[1]);
#endif
				dp_VIA_t1_cnt_lo = mine->scale;
				Draw_VLp(mine->world_vlist);
			}
			else if (mine->state == MINE_STATE_EXPLODE)
			{
				Moveto_d(mine->obj.center_pos[0], mine->obj.center_pos[1]);
				dp_VIA_t1_cnt_lo = 0x10 + (mine->hi_counter << 3);
				Draw_VLp((signed char *) mine_explode);
			}
		}

		mine = (struct mine *) mine->obj.elmnt.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
