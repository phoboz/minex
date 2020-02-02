// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "bullet.h"

// ---------------------------------------------------------------------------

unsigned int num_bullets = 0;
struct bullet bullets[MAX_BULLETS];

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int speed
	)
{
	bullet->active = 1;
	bullet->world_pos[0] = y;
	bullet->world_pos[1] = x;
	bullet->speed = speed;
	num_bullets++;
}

void deinit_bullet(
	struct bullet *bullet
	)
{
	bullet->active = 0;
	num_bullets--;
}

void fire_bullet(
	signed int y,
	signed int x,
	signed int speed
	)
{
	unsigned int i;
	struct bullet *bullet;

	if (num_bullets < MAX_BULLETS)
	{
		for (i = 0; i < MAX_BULLETS; i++)
		{
			bullet = &bullets[i];
			if (!bullet->active)
			{
				init_bullet(bullet, y, x, speed);
				break;
			}
		}
	}
}

void move_bullets(void)
{
	unsigned int i;
	struct bullet *bullet;

	for (i = 0; i < MAX_BULLETS; i++)
	{
		bullet = &bullets[i];
		if (bullet->active)
		{
			if (bullet->speed)
			{
				bullet->world_pos[0] += bullet->speed;
			}
	
			if (!(bullet->world_pos[0] >= OBJECT_MIN_Y && bullet->world_pos[0] <= OBJECT_MAX_Y &&
		     	 bullet->world_pos[1] >= OBJECT_MIN_X && bullet->world_pos[1] <= OBJECT_MAX_X))
			{
				deinit_bullet(bullet);
			}
		}
	}
}

unsigned int hit_object_bullet(
	struct bullet *bullet,
	struct object *obj
	)
{
	signed int bullet_y, bullet_x;
	signed int obj_center_y, obj_center_x;
	signed int obj_y1, obj_x1, obj_y2, obj_x2;

	unsigned int result = 0;

	bullet_y = bullet->world_pos[0];
	bullet_x = bullet->world_pos[1];

	obj_center_y = obj->world_pos[0] + obj->center_pos[0];
	obj_center_x = obj->world_pos[1] + obj->center_pos[1];

	obj_y1 = obj_center_y + obj->dim_2[0];
	obj_x1 = obj_center_x - obj->dim_2[1];
	obj_y2 = obj_center_y - obj->dim_2[0];
	obj_x2 = obj_center_x + obj->dim_2[1];

	if (bullet_y > obj_y1 && bullet_y < obj_y2 && bullet_x > obj_x1 && bullet_x < obj_x2)
	{
		result = 1;
	}

	return result;
}

void draw_bullets(void)
{
	unsigned int i;
	struct bullet *bullet;

	for (i = 0; i < MAX_BULLETS; i++)
	{
		bullet = &bullets[i];
		if (bullet->active)
		{
			Reset0Ref();
			Moveto_d(0, 0);
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
			Dot_d(bullet->world_pos[0], bullet->world_pos[1]);
		}
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
