// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
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
	init_object(&mine->obj, y, x, h, w, scale, shape, &mine_list);

	mine->world_angle		= world_angle;
	mine->old_world_angle	= world_angle;
	Rot_VL_ab(world_angle, 0, mine->obj.pos, mine->obj.world_pos);
	Rot_VL_Mode(world_angle, (signed int*) mine->obj.shape, &mine->world_vlist);
}

void deinit_mine(
	struct mine *mine
	)
{
	deinit_object(&mine->obj, &mine_list);
	give_object(&mine->obj, &mine_free_list);
}

void move_mines(void)
{
	struct mine *mine;
	struct mine *rem = 0;

	mine = (struct mine *) mine_list;
	while (mine != 0)
	{
		if (mine->world_angle != mine->old_world_angle)
		{
			Rot_VL_ab(mine->world_angle, 0, mine->obj.pos, mine->obj.world_pos);
			Rot_VL_Mode(mine->world_angle, (signed int *) mine->obj.shape, mine->world_vlist);
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
		Reset0Ref();
		Moveto_d(0, 0);

		dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
		Moveto_d(mine->obj.world_pos[0], mine->obj.world_pos[1]);
		dp_VIA_t1_cnt_lo = mine->obj.scale;
		Draw_VLp(mine->world_vlist);
		mine = (struct mine *) mine->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
