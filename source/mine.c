// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "random.h"
#include "mine.h"

// ---------------------------------------------------------------------------

struct object *mine_list = 0;
struct object *mine_free_list = 0;

void init_mine(
	struct mine *mine,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int type,
	unsigned int treshold,
	unsigned int world_angle,
	unsigned int scale,
	const signed char *shape
	)
{
	take_object(&mine->obj, &mine_free_list);
	init_object(&mine->obj, y, x, h, w, &mine_list);
	mine->scale = scale;
	mine->shape = shape;

	mine->obj_pos[0]	= y;
	mine->obj_pos[1]	= x;

	mine->velocity[0]	= 0;
	mine->velocity[1]	= 0;

	mine->type		= type;
	mine->state		= MINE_STATE_IDLE;
	mine->lo_counter	= 0;
	mine->hi_counter	= 0;
	mine->treshold	= treshold;

	mine->world_angle = world_angle;
	Rot_VL_ab(world_angle, 0, mine->obj.dim_2, mine->obj.center_pos);
	Rot_VL_ab(world_angle, 0, mine->obj.rel_pos, mine->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) shape, &mine->world_vlist);
}

void deinit_mine(
	struct mine *mine
	)
{
	deinit_object(&mine->obj, &mine_list);
	give_object(&mine->obj, &mine_free_list);
}

void move_mines(
	struct player *player
	)
{
	unsigned int mine_index = 0;
	unsigned int update_view;

	struct mine *mine;
	struct mine *rem = 0;

	struct bullet *bullet;

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
					mine->obj.rel_pos[0] = mine->obj_pos[0] - player->obj.rel_pos[0];
					mine->obj.rel_pos[1] = mine->obj_pos[1] + player->obj.rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->obj.rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						//if (mine->state == MINE_STATE_ACTIVE)
						{
							Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
						}
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
					mine->obj.rel_pos[0] = mine->obj_pos[0] - player->obj.rel_pos[0];
					mine->obj.rel_pos[1] = mine->obj_pos[1] + player->obj.rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->obj.rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						//if (mine->state == MINE_STATE_ACTIVE)
						{
							Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
						}
					}

					mine->world_angle = player->angle;
				}
			}
		}
		else if (mine->lo_counter == 3)
		{
			mine->lo_counter = 0;

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
					mine->obj.rel_pos[0] = mine->obj_pos[0] - player->obj.rel_pos[0];
					mine->obj.rel_pos[1] = mine->obj_pos[1] + player->obj.rel_pos[1];
					Rot_VL_ab(player->angle, 0, mine->obj.dim_2, mine->obj.center_pos);
					Rot_VL_ab(player->angle, 0, mine->obj.rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						//if (mine->state == MINE_STATE_ACTIVE)
						{
							Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
						}
					}

					mine->world_angle = player->angle;
				}
			}
		}

		bullet = (struct bullet *) bullet_list;
		while (bullet)
		{
			if (hit_particle_object(&bullet->obj, &mine->obj))
			{
				rem = mine;
			}

			bullet = (struct bullet *) bullet->obj.next;
		}

		mine = (struct mine *) mine->obj.next;
		mine_index++;

		if (rem != 0)
		{
			deinit_mine(rem);
			rem = 0;
		}
	}
}

void draw_mines(void)
{
	struct mine *mine;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		if (mine->obj.rel_pos[0] >= OBJECT_MIN_Y && mine->obj.rel_pos[0] <= OBJECT_MAX_Y &&
		    mine->obj.rel_pos[1] >= OBJECT_MIN_X && mine->obj.rel_pos[1] <= OBJECT_MAX_X)
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
		}

		mine = (struct mine *) mine->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
