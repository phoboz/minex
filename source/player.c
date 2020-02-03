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

	player->speed_counter	= 0;
	player->speed			= 0;

	player->fire_countdown = 0;

	Rot_VL_ab(angle, 0, (signed int *) player_up_vec, player->up_vec);
}

void move_player(
	struct player *player
	)
{
	struct bullet *bullet;

	player->update_view = 0;

	if (player->fire_countdown)
	{
		player->fire_countdown--;
	}

	check_joysticks();
	check_buttons();

	if (joystick_1_left())
	{
		if (--player->angle == 0)
		{
			player->angle = 64;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
		player->update_view = 1;
	}
	else if (joystick_1_right())
	{
		if (++player->angle == 64)
		{
			player->angle = 0;
		}

		Rot_VL_ab(player->angle, 0, (signed int *) player_up_vec, player->up_vec);
		player->update_view = 1;
	}

	if (button_1_4_pressed())
	{
		if (!player->fire_countdown)
		{
			bullet = (struct bullet *) bullet_free_list;
			if (bullet)
			{
				init_bullet(bullet, 0, 0, PLAYER_BULLET_SPEED);
				player->fire_countdown = PLAYER_FIRE_TRESHOLD;
			}
		}
	}

	if (button_1_3_held())
	{
		if (++player->speed_counter >= PLAYER_ACCELERATE_TRESHOLD)
		{
			player->speed_counter = 0;
			if (player->speed < PLAYER_MAX_SPEED)
			{
				player->speed++;
			}
		}
	}
	else
	{
		if (++player->speed_counter >= PLAYER_BRAKE_TRESHOLD)
		{
			player->speed_counter = 0;
			if (player->speed > 0)
			{
				player->speed--;
			}
		}
	}

	if (player->speed)
	{
		player->rel_pos[0] += player->up_vec[0] * player->speed;
		player->rel_pos[1] += player->up_vec[1] * player->speed;
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
