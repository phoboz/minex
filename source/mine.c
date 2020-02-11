// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"
#include "random.h"
#include "wrap.h"
#include "imath.h"
#include "mine_data.h"
#include "mine.h"

// ---------------------------------------------------------------------------

struct element *mine_list = 0;
struct element *mine_free_list = 0;

void init_mine(
	struct mine *mine,
	signed int y,
	signed int x,
	unsigned int type,
	unsigned int size,
	unsigned int treshold,
	struct player *player
	)
{
	take_element(&mine->obj.elmnt, &mine_free_list);

	if ((type & MINE_TYPE_MAGNETIC) && (type & MINE_TYPE_FIREBALL))
	{
		init_object(&mine->obj, y, x, mine_3_sizes[size], mine_3_sizes[size], &mine_list);
		mine->shape = mine_3[size];
	}
	else if (type & MINE_TYPE_MAGNETIC)
	{
		init_object(&mine->obj, y, x, mine_1_sizes[size], mine_1_sizes[size], &mine_list);
		mine->shape = mine_1[size];
	}
	else if (type & MINE_TYPE_FIREBALL)
	{
		init_object(&mine->obj, y, x, mine_2_sizes[size], mine_2_sizes[size], &mine_list);
		mine->shape = mine_2[size];
	}
	else
	{
		init_object(&mine->obj, y, x, mine_0_sizes[size], mine_0_sizes[size], &mine_list);
		mine->shape = mine_0[size];
	}

	mine->obj_pos[0]	= y;
	mine->obj_pos[1]	= x;

	mine->rel_pos[0] = y;
	mine->rel_pos[1] = x;

	mine->velocity[0]	= 0;
	mine->velocity[1]	= 0;

	mine->type_size	= type | size;
	mine->state		= MINE_STATE_IDLE;
	mine->lo_counter	= 0;
	mine->hi_counter	= 0;
	mine->treshold	= treshold;

	mine->world_angle = player->angle;
	wrap_translate(mine->rel_pos, mine->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
	Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);
	Rot_VL_Mode(player->angle, (signed int*) mine->shape, &mine->world_vlist);
}

void deinit_mine(
	struct mine *mine
	)
{
	deinit_object(&mine->obj, &mine_list);
	give_element(&mine->obj.elmnt, &mine_free_list);
}

void rand_dir_mine(
	struct mine *mine
	)
{
	unsigned int size = mine->type_size & MINE_SIZE_MASK;

	if (random() & 2)
	{
		mine->velocity[0] = (signed int) (3 - size);
	}
	else
	{
		mine->velocity[0] = -(signed int) (3 - size);
	}

	if (random() & 2)
	{
		mine->velocity[1] = (signed int) (3 - size);
	}
	else
	{
		mine->velocity[1] = -(signed int) (3 - size);
	}
}

void target_player_mine(
	struct mine *mine,
	struct player *player,
	signed int speed
	)
{
	signed int v[2];
	signed int p[2];
	unsigned long len;

	p[0] = player->obj_pos[0];
	p[1] = -player->obj_pos[1];

	wrap_translate(v, p, -mine->obj_pos[0], -mine->obj_pos[1]);
	len = isqrt16((unsigned long) v[0] * (unsigned long) v[0] + (unsigned long) v[1] * (unsigned long) v[1]);
	if (len >= MINE_TARGET_MIN_RANGE)
	{
		mine->velocity[0] = (signed int) (v[0] * 	(signed long) speed / (signed long) len);
		mine->velocity[1] = (signed int) (v[1] * (signed long) speed / (signed long) len);
	}
	else
	{
		mine->velocity[0] = 0;
		mine->velocity[1] = 0;
	}
}

unsigned int move_mines(
	struct player *player
	)
{
	unsigned int status = 0;
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
				if (mine->state == MINE_STATE_ACTIVE || mine->state == MINE_STATE_FIREBALL)
				{
					if (++mine->hi_counter == MINE_TARGET_TRESHOLD)
					{
						mine->hi_counter = 0;
						if ((mine->type_size & MINE_TYPE_MAGNETIC) && mine->state == MINE_STATE_ACTIVE)
						{
							target_player_mine(mine, player, 4 - (signed int) (mine->type_size & MINE_SIZE_MASK));
						}
					}

					if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
					{
						wrap_translate(mine->obj_pos, mine->obj_pos, mine->velocity[0], mine->velocity[1]);
						update_view = 1;
					}
				}

				if (update_view || player->update_view)
				{
					wrap_translate(mine->rel_pos, mine->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
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
				if (mine->state == MINE_STATE_ACTIVE || mine->state == MINE_STATE_FIREBALL)
				{
					if (++mine->hi_counter == MINE_TARGET_TRESHOLD)
					{
						mine->hi_counter = 0;
						if ((mine->type_size & MINE_TYPE_MAGNETIC) && mine->state == MINE_STATE_ACTIVE)
						{
							target_player_mine(mine, player, 4 - (signed int) (mine->type_size & MINE_SIZE_MASK));
						}
					}

					if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
					{
						wrap_translate(mine->obj_pos, mine->obj_pos, mine->velocity[0], mine->velocity[1]);
						update_view = 1;
					}
				}

				if (update_view || player->update_view)
				{
					wrap_translate(mine->rel_pos, mine->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
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

					mine->state = MINE_STATE_ACTIVATE;
					if (mine->type_size & MINE_TYPE_MAGNETIC)
					{
						target_player_mine(mine, player, 4 - (signed int) (mine->type_size & MINE_SIZE_MASK));
					}
					else
					{
						rand_dir_mine(mine);
					}
					update_view = 1;
				}
			}
			else if (mine->state == MINE_STATE_ACTIVATE)
			{
				if (++mine->hi_counter == MINE_ACTIVATE_TRESHOLD)
				{
					mine->hi_counter = 0;
					mine->state = MINE_STATE_ACTIVE;
				}
			}
			else if (mine->state == MINE_STATE_EXPLODE)
			{
				if (++mine->hi_counter == MINE_EXPLODE_TRESHOLD)
				{
					mine->hi_counter = 0;
					if (mine->type_size & MINE_TYPE_FIREBALL)
					{
						target_player_mine(mine, player, MINE_FIREBALL_SPEED);
						mine->state = MINE_STATE_FIREBALL;
					}
					else
					{
						mine->state = MINE_STATE_REMOVE;
					}
				}
			}

			if (mine_index == 2 || mine_index == 5 || mine_index == 8 || mine_index == 11)
			{
				if (mine->state == MINE_STATE_ACTIVE || mine->state == MINE_STATE_FIREBALL)
				{
					if (++mine->hi_counter == MINE_TARGET_TRESHOLD)
					{
						mine->hi_counter = 0;
						if ((mine->type_size & MINE_TYPE_MAGNETIC) && mine->state == MINE_STATE_ACTIVE)
						{
							target_player_mine(mine, player, 4 - (signed int) (mine->type_size & MINE_SIZE_MASK));
						}
					}

					if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
					{
						wrap_translate(mine->obj_pos, mine->obj_pos, mine->velocity[0], mine->velocity[1]);
						update_view = 1;
					}
				}

				if (update_view || player->update_view)
				{
					wrap_translate(mine->rel_pos, mine->obj_pos, - player->obj_pos[0], player->obj_pos[1]);
					Rot_VL_ab(player->angle, 0, mine->rel_pos, mine->obj.world_pos);

					if (mine->world_angle != player->angle)
					{
						Rot_VL_Mode(player->angle, (signed int *) mine->shape, mine->world_vlist);
					}

					mine->world_angle = player->angle;
				}
			}
		}

		if (mine->state == MINE_STATE_ACTIVE || mine->state == MINE_STATE_FIREBALL)
		{
			bullet = (struct bullet *) bullet_list;
			while (bullet)
			{
				if (mine->state == MINE_STATE_ACTIVE)
				{
					if (hit_object_bullet(bullet, &mine->obj))
					{
						mine->state = MINE_STATE_EXPLODE;
						mine->velocity[0] = mine->velocity[1] = 0;
						rem_bullet = bullet;
						status |= MINE_STATUS_EXPLODE;
					}
				}
				else if (mine->state == MINE_STATE_FIREBALL) 
				{
					if (hit_dim_object_bullet(bullet, &mine->obj, -FIREBALL_SIZE, FIREBALL_SIZE))
					{
						mine->state = MINE_STATE_REMOVE;
						mine->velocity[0] = mine->velocity[1] = 0;
						rem_bullet = bullet;
					}
				}

				bullet = (struct bullet *) bullet->elmnt.next;

				if (rem_bullet != 0)
				{
					deinit_bullet(rem_bullet);
					rem_bullet = 0;
				}
			}

#ifndef NO_HIT
			if (player->anim.obj.active)
			{
				if (mine->state == MINE_STATE_ACTIVE)
				{
					if (check_box_object(
						&mine->obj,
						player->anim.obj.dim_2[0],
						-player->anim.obj.dim_2[1],
						-player->anim.obj.dim_2[0],
						player->anim.obj.dim_2[1]
						))
					{
						mine->state = MINE_STATE_EXPLODE;
						hit_player(player);
						status = MINE_STATUS_EXPLODE;
					}
				}
				else if (mine->state == MINE_STATE_FIREBALL)
				{
					if (check_box_dim_object(
						&mine->obj,
						-FIREBALL_SIZE,
						FIREBALL_SIZE,
						player->anim.obj.dim_2[0],
						-player->anim.obj.dim_2[1],
						-player->anim.obj.dim_2[0],
						player->anim.obj.dim_2[1]
						))
					{
						mine->state = MINE_STATE_REMOVE;
						hit_player(player);
						status = MINE_STATUS_EXPLODE;
					}
				}
			}
#endif

			if (mine->state == MINE_STATE_FIREBALL)
			{
				if ((mine->velocity[0] == 0 && mine->velocity[1] == 0) ||
					mine->obj.world_pos[0] < OBJECT_MIN_Y || mine->obj.world_pos[0] > OBJECT_MAX_Y ||
					mine->obj.world_pos[1] < OBJECT_MIN_X || mine->obj.world_pos[1] > OBJECT_MAX_X)
				{
					mine->state = MINE_STATE_REMOVE;
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

	return status;
}

void draw_mines(void)
{
	struct mine *mine;
	signed int y, x;
	signed int h, w;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		y = mine->obj.world_pos[0];
		x = mine->obj.world_pos[1];
		h = -mine->obj.dim_2[0];
		w = mine->obj.dim_2[1];
		if (!(y + h < OBJECT_MIN_Y || y - h > OBJECT_MAX_Y ||
			x + w < OBJECT_MIN_X || x - w > OBJECT_MAX_X))
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Moveto_d(y, x);

			if (mine->state == MINE_STATE_IDLE)
			{
				Dot_here();
			}
			else if (mine->state == MINE_STATE_ACTIVATE)
			{
				dp_VIA_t1_cnt_lo = (MINE_DRAW_SCALE - MINE_ACTIVATE_TRESHOLD) + mine->hi_counter;
				Draw_VLp(mine->world_vlist);
			}
			else if (mine->state == MINE_STATE_ACTIVE)
			{
//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
				Moveto_d(-mine->obj.dim_2[0], -mine->obj.dim_2[1]);

				Draw_Line_d(0, mine->obj.dim_2[1] << 1);
				Draw_Line_d(mine->obj.dim_2[0] << 1, 0);
				Draw_Line_d(0, -mine->obj.dim_2[1] << 1);
				Draw_Line_d(-mine->obj.dim_2[0] << 1, 0);

				Moveto_d(mine->obj.dim_2[0], mine->obj.dim_2[1]);
#endif
				if (mine->type_size & MINE_TYPE_FIREBALL)
				{
					Dot_here();
				}

				dp_VIA_t1_cnt_lo = MINE_DRAW_SCALE;
				Draw_VLp(mine->world_vlist);
			}
			else if (mine->state == MINE_STATE_EXPLODE)
			{
				dp_VIA_t1_cnt_lo = 0x10 + (mine->hi_counter << 3);
				Draw_VLp((signed char *) mine_explode[mine->type_size & MINE_SIZE_MASK]);
			}
			else if (mine->state == MINE_STATE_FIREBALL)
			{
#ifdef DEBUG_DRAW
				Moveto_d(-FIREBALL_SIZE, -FIREBALL_SIZE);

				Draw_Line_d(0, FIREBALL_SIZE << 1);
				Draw_Line_d(FIREBALL_SIZE << 1, 0);
				Draw_Line_d(0, -FIREBALL_SIZE << 1);
				Draw_Line_d(-FIREBALL_SIZE << 1, 0);

				Moveto_d(FIREBALL_SIZE, FIREBALL_SIZE);
#endif

				dp_VIA_t1_cnt_lo = FIREBALL_DRAW_SCALE;
				Draw_VLp((signed char *) fireball);
			}
		}

		mine = (struct mine *) mine->obj.elmnt.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
