// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "controller.h"
#include "bullet.h"
#include "wrap.h"
#include "imath.h"
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
	const signed char * const *shapes
	)
{
	init_animation(&player->anim, y, x, h, w, scale, shapes);

	player->angle	= angle;

	player->speed_counter	= 0;
	player->speed			= 0;

	player->fire_countdown = 0;

	player->state = PLAYER_STATE_NORMAL;

	Rot_VL_ab(angle, 0, (signed int *) player_up_vec, player->up_vec);
}

unsigned int move_player(
	struct player *player
	)
{
	struct bullet *bullet;

	unsigned int status = 0;

	player->update_view = 0;

	if (player->anim.obj.active)
	{
		if (player->state == PLAYER_STATE_NORMAL)
		{
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
						status |= PLAYER_STATUS_FIRE;
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

				player->anim.base_frame = (unsigned int) abs(player->speed);
				if (player->speed == PLAYER_MAX_SPEED)
				{
					if (++player->anim_counter >= PLAYER_FLAME_ANIM_TRESHOLD)
					{
						player->anim_counter = 0;
						if (++player->anim.frame >= PLAYER_FLAME_NUM_FRAMES)
						{
							player->anim.frame = 0;
						}
					}
				}
				else
				{
					player->anim_counter = 0;
					player->anim.frame = 0;
				}

				status |= PLAYER_STATUS_THRUST;
			}
			else if (player->speed)
			{
				player->anim.base_frame = 0;
				player->anim.frame = 0;

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
				wrap_translate(
					player->rel_pos,
					player->rel_pos,
					player->up_vec[0] * player->speed,
					player->up_vec[1] * player->speed
					);
				player->update_view = 1;
			}
		}
		else if (player->state == PLAYER_STATE_EXPLODE)
		{
			if (++player->anim_counter >= PLAYER_EXPLODE_ANIM_TRESHOLD)
			{
				player->anim_counter = 0;
				if (++player->anim.frame >= PLAYER_EXPLODE_NUM_FRAMES)
				{
					player->anim.frame = 0;
					player->state = PLAYER_STATE_REMOVED;
					player->anim.obj.active = 0;
				}
			}
		}
	}

	return status;
}

void hit_player(
	struct player *player
	)
{
	if (player->state != PLAYER_STATE_EXPLODE)
	{
		player->anim_counter = 0;
		player->anim.base_frame = PLAYER_EXPLODE_FRAME;
		player->state = PLAYER_STATE_EXPLODE;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
