// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player.h"
#include "wrap.h"
#include "bullet.h"

// ---------------------------------------------------------------------------

struct element *bullet_list = 0;
struct element *bullet_free_list = 0;

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int speed
	)
{
	take_element(&bullet->elmnt, &bullet_free_list);
	give_element(&bullet->elmnt, &bullet_list);

	bullet->world_pos[0] = y;
	bullet->world_pos[1] = x;

	bullet->speed = speed;
}

void deinit_bullet(
	struct bullet *bullet
	)
{
	take_element(&bullet->elmnt, &bullet_list);
	give_element(&bullet->elmnt, &bullet_free_list);
}

void move_bullets(void)
{
	struct bullet *bullet;
	struct bullet *rem_bullet = 0;

	bullet = (struct bullet *) bullet_list;
	while (bullet != 0)
	{
		if (bullet->speed)
		{
			bullet->world_pos[0] += bullet->speed;
		}
	
		if (bullet->world_pos[0] < OBJECT_MIN_Y || bullet->world_pos[0] > OBJECT_MAX_Y)
		{
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

void draw_bullets(void)
{
	struct bullet *bullet;

	bullet = (struct bullet *) bullet_list;
	while (bullet != 0)
	{
		Reset0Ref();
		Moveto_d(0, 0);
		dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;
		Dot_d(bullet->world_pos[0], bullet->world_pos[1]);

		bullet = (struct bullet *) bullet->elmnt.next;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
