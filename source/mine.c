// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
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

	mine->world_angle = world_angle;
	Rot_VL_ab(world_angle, 0, mine->obj.pos, mine->obj.world_pos);
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
	unsigned int update_view;

	struct mine *mine;
	struct mine *rem = 0;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		update_view = 0;

		if (mine->velocity[0] != 0 || mine->velocity[1] != 0)
		{
			mine->obj_pos[0] += mine->velocity[0];
			mine->obj_pos[1] += mine->velocity[1];
			update_view = 1;
		}

		if (update_view || player->update_view)
		{
			mine->world_angle = player->angle;
			mine->obj.pos[0] = mine->obj_pos[0] - player->obj.pos[0];
			mine->obj.pos[1] = mine->obj_pos[1] + player->obj.pos[1];

			Rot_VL_ab(mine->world_angle, 0, mine->obj.pos, mine->obj.world_pos);
			Rot_VL_Mode(mine->world_angle, (signed int *) mine->shape, mine->world_vlist);
		}
	
		if (/*remove*/0)
		{
			rem = mine;
		}

		mine = (struct mine *) mine->obj.next;

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
		if (mine->obj.pos[0] >= OBJECT_MIN_Y && mine->obj.pos[0] <= OBJECT_MAX_Y &&
		    mine->obj.pos[1] >= OBJECT_MIN_X && mine->obj.pos[1] <= OBJECT_MAX_X)
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Moveto_d(mine->obj.world_pos[0], mine->obj.world_pos[1]);

			dp_VIA_t1_cnt_lo = mine->scale;
			Draw_VLp(mine->world_vlist);
		}

		mine = (struct mine *) mine->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
