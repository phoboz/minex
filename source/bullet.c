// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"

// ---------------------------------------------------------------------------

struct object *bullet_list = 0;
struct object *bullet_free_list = 0;

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	signed int speed
	)
{
	take_object(&bullet->obj, &bullet_free_list);
	init_object(&bullet->obj, y, x, h, w, &bullet_list);

	bullet->speed = speed;
}

void deinit_bullet(
	struct bullet *bullet
	)
{
	deinit_object(&bullet->obj, &bullet_list);
	give_object(&bullet->obj, &bullet_free_list);
}

void move_bullets(void)
{
	struct bullet *bullet;
	struct bullet *rem = 0;

	bullet = (struct bullet *) bullet_list;
	while (bullet != 0)
	{
		if (bullet->speed)
		{
			bullet->obj.world_pos[0] += bullet->speed;
		}
	
		if (!(bullet->obj.world_pos[0] >= OBJECT_MIN_Y && bullet->obj.world_pos[0] <= OBJECT_MAX_Y &&
		      bullet->obj.world_pos[1] >= OBJECT_MIN_X && bullet->obj.world_pos[1] <= OBJECT_MAX_X))
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
		Reset0Ref();
		Moveto_d(0, 0);
		dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
		Dot_d(bullet->obj.world_pos[0], bullet->obj.world_pos[1]);

		bullet = (struct bullet *) bullet->obj.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
