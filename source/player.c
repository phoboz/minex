// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "controller.h"
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
	init_object(&player->obj, y, x, h, w, scale, shape, 0);

	player->angle	= angle;
	player->speed	= 0;
}

void move_player(
	struct player *player
	)
{
	check_joysticks();
	check_buttons();

	if (joystick_1_left())
	{
		if (--player->angle == 0)
		{
			player->angle = 255;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
	}
	else if (joystick_1_right())
	{
		if (++player->angle == 255)
		{
			player->angle = 0;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
	}

	if (button_1_3_held())
	{
		player->speed = 1;
	}
	else
	{
		player->speed = 0;
	}

	if (player->speed)
	{
		player->obj.pos[0] += player->up_vec[0] * player->speed;
		player->obj.pos[1] += player->up_vec[1] * player->speed;
	}
}

void draw_player(
	struct player *player
	)
{
	Reset0Ref();
	Moveto_d(0, 0);

	dp_VIA_t1_cnt_lo = player->obj.scale;
	Draw_VLp((signed int *) player->obj.shape);
}

// ***************************************************************************
// end of file
// ***************************************************************************
