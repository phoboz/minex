// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"

// ---------------------------------------------------------------------------

struct object *bullet_list = 0;
struct object *bullet_free_list = 0;

static const signed char bullet_up_vec[]=
{
	1,	0
};

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	signed int speed,
	unsigned int world_angle
	)
{
	take_object(&bullet->obj, &bullet_free_list);
	init_object(&bullet->obj, y, x, h, w, &bullet_list);

	bullet->obj_pos[0]	= y;
	bullet->obj_pos[1]	= x;

	bullet->world_angle = world_angle;
	Rot_VL_ab(world_angle, 0, bullet->obj.pos, bullet->obj.world_pos);

	bullet->speed = speed;
	bullet->velocity[0] = bullet_up_vec[0];
	bullet->velocity[1] = bullet_up_vec[1];
}

void deinit_bullet(
	struct bullet *bullet
	)
{
	deinit_object(&bullet->obj, &bullet_list);
	give_object(&bullet->obj, &bullet_free_list);
}

void move_bullets(
	struct player *player
	)
{
	unsigned int update_view;

	struct bullet *bullet;
	struct bullet *rem = 0;

	bullet = (struct bullet *) bullet_list;
	while (bullet != 0)
	{
		update_view = 0;

		if (bullet->velocity[0] != 0 || bullet->velocity[1] != 0)
		{
			bullet->obj_pos[0] += bullet->speed * bullet->velocity[0];
			bullet->obj_pos[1] += bullet->speed * bullet->velocity[1];
			update_view = 1;
		}

		if (update_view || player->update_view)
		{
			bullet->world_angle = player->angle;
			bullet->obj.pos[0] = bullet->obj_pos[0] - player->obj.pos[0];
			bullet->obj.pos[1] = bullet->obj_pos[1] + player->obj.pos[1];

			Rot_VL_ab(0/*bullet->world_angle*/, 0, bullet->obj.pos, bullet->obj.world_pos);
		}
	
		if (!(bullet->obj.pos[0] >= OBJECT_MIN_Y && bullet->obj.pos[0] <= OBJECT_MAX_Y &&
		      bullet->obj.pos[1] >= OBJECT_MIN_X && bullet->obj.pos[1] <= OBJECT_MAX_X))
		{
			rem = bullet;
		}

		bullet = (struct bullet *) bullet->obj.next;

		if (rem != 0)
		{
			deinit_bullet(rem);
			rem = 0;
		}
	}
}

void draw_bullets(void)
{
	struct bullet *bullet;

	bullet = (struct bullet *) bullet_list;
	while (bullet != 0)
	{
		if (bullet->obj.pos[0] >= OBJECT_MIN_Y && bullet->obj.pos[0] <= OBJECT_MAX_Y &&
		    bullet->obj.pos[1] >= OBJECT_MIN_X && bullet->obj.pos[1] <= OBJECT_MAX_X)
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Dot_d(bullet->obj.world_pos[0], bullet->obj.world_pos[1]);
		}

		bullet = (struct bullet *) bullet->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
