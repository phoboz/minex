// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "controller.h"
#include "bullet.h"
#include "player.h"

// ---------------------------------------------------------------------------

static const signed char player_up_vec[]=
{
	1,	0
};

void init_player(
	struct player *player,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int angle,
	unsigned int scale,
	const signed char *shape
	)
{
	init_object(&player->obj, y, x, h, w, 0);
	player->shape = shape;
	player->scale = scale;

	player->angle	= angle;
	player->speed	= 0;

	Rot_VL_ab(angle, 0, (signed int *) player_up_vec, player->up_vec);
}

extern struct bullet bullets[];
static struct bullet* get_bullet(void)
{
	struct bullet *bullet = 0;
	unsigned int i;

	for (i = 0; i < 5; i++)
	{
		if (!bullets[i].obj.active)
		{
			bullet = &bullets[i];
		}
	}

	return bullet;
}

void move_player(
	struct player *player
	)
{
	struct bullet *bullet;

	player->update_view = 0;

	check_joysticks();
	check_buttons();

	if (joystick_1_left())
	{
		if (--player->angle == 0)
		{
			player->angle = 255;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
		player->update_view = 1;
	}
	else if (joystick_1_right())
	{
		if (++player->angle == 255)
		{
			player->angle = 0;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
		player->update_view = 1;
	}

	if (button_1_4_pressed())
	{
		bullet = get_bullet();
		if (bullet)
		{
			init_bullet(bullet, player->obj.pos[0] + player->obj.h_2, -player->obj.pos[1], 2, 2, 1, player->up_vec, player->angle);
		}
	}

	if (button_1_3_held())
	{
		player->speed = 2;
	}
	else
	{
		player->speed = 0;
	}

	if (player->speed)
	{
		player->obj.pos[0] += player->up_vec[0] * player->speed;
		player->obj.pos[1] += player->up_vec[1] * player->speed;
		player->update_view = 1;
	}
}

void draw_player(
	struct player *player
	)
{
	Reset0Ref();
	Moveto_d(0, 0);
	dp_VIA_t1_cnt_lo = player->scale;
	Draw_VLp((signed int *) player->shape);
}

// ***************************************************************************
// end of file
// ***************************************************************************
