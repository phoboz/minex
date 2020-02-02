// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "random.h"
#include "mine.h"

// ---------------------------------------------------------------------------

unsigned int num_mines = 0;
struct mine mines[MAX_MINES];

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
	init_object(&mine->obj, y, x, h, w);
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

	mine->world_angle = world_angle;
	Rot_VL_ab(world_angle, 0, mine->obj.dim_2, mine->obj.center_pos);
	Rot_VL_ab(world_angle, 0, mine->rel_pos, mine->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) shape, &mine->world_vlist);

	num_mines++;
}

void deinit_mine(
	struct mine *mine
	)
{
	deinit_object(&mine->obj);
	num_mines--;
}

void move_mines(
	struct player *player
	)
{
	unsigned int i;
	unsigned int update_view;
	struct mine *mine;
	struct bullet *bullet;

	for (i = 0; i < MAX_MINES; i++)
	{
		mine = &mines[i];
		if (mine->obj.active)
		{
			update_view = 0;

			mine->lo_counter++;
			if (mine->lo_counter == 1)
			{
				if (i == 0 || i == 3 || i == 6 || i == 9)
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
				if (i == 1 || i == 4 || i == 7 || i == 10)
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

				if (i == 2 || i == 5 || i == 8 || i == 11)
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
				unsigned int j;
				for (j = 0; j < MAX_BULLETS; j++)
				{
					bullet = &bullets[j];
					if (bullet->active)
					{
						if (hit_object_bullet(bullet, &mine->obj))
						{
							deinit_mine(mine);
							deinit_bullet(bullet);
						}
					}
				}
			}
		}
	}
}

void draw_mines(void)
{
	unsigned int i;
	struct mine *mine;
	signed int center_y, center_x;

	for (i = 0; i < MAX_MINES; i++)
	{
		mine = &mines[i];
		if (mine->obj.active)
		{
			center_y = mine->obj.world_pos[0] + mine->obj.center_pos[0];
			center_x = mine->obj.world_pos[1] + mine->obj.center_pos[1];
			if (center_y >= OBJECT_MIN_Y && center_y <= OBJECT_MAX_Y &&
				center_y >= OBJECT_MIN_X && center_y <= OBJECT_MAX_X)
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
		}
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
