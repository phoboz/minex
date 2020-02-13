// ***************************************************************************
// main
// ***************************************************************************
// This file was developed by Prof. Dr. Peer Johannsen as part of the 
// "Retro-Programming" and "Advanced C Programming" class at
// Pforzheim University, Germany.
// 
// It can freely be used, but at one's own risk and for non-commercial
// purposes only. Please respect the copyright and credit the origin of
// this file.
//
// Feedback, suggestions and bug-reports are welcome and can be sent to:
// peer.johannsen@pforzheim-university.de
// ---------------------------------------------------------------------------

#include <vectrex.h>
#include "ayfxPlayer.h"
#include "controller.h"
#include "player.h"
#include "player_data.h"
#include "mine.h"
#include "mine_data.h"
#include "ship.h"
#include "ship_data.h"
#include "bullet.h"
#include "random.h"
#include "wave.h"
#include "text.h"

#define GAME_STATE_NORMAL		0
#define GAME_STATE_NEXT_LEVEL	1
#define GAME_STATE_HYPERSPACE	2

extern const unsigned int bullet_snd_data[];
extern const unsigned int explosion_snd_data[];
extern const unsigned int thrust_snd_data[];

struct player player;

static unsigned int game_state;
static unsigned long game_seed;
static unsigned int anim_counter;
static unsigned int anim_frame;

// ---------------------------------------------------------------------------
// cold reset: the vectrex logo is shown, all ram data is cleared
// warm reset: skip vectrex logo and keep ram data
// ---------------------------------------------------------------------------
// at system startup, when powering up the vectrex, a cold reset is performed
// if the reset button is pressed, then a warm reset is performed
// ---------------------------------------------------------------------------
// after each reset, the cartridge title is shown and then main() is called
// ---------------------------------------------------------------------------

int main(void)
{
	unsigned int player_status;
	unsigned int enemy_status;

	enable_controller_1_x();
	enable_controller_1_y();
	disable_controller_2_x();
	disable_controller_2_y();

	game_seed = (unsigned long) 2458433;

	init_random(5, 27, 3, 19);
	random_long_seed(game_seed);

	init_wave();

	init_player(&player, 0, 0, PLAYER_HEIGHT, PLAYER_WIDTH, 0, PLAYER_DRAW_SCALE, player_anim);
	player.score = 0;
	player.extra_lives = 3;

	game_state = GAME_STATE_NEXT_LEVEL;
	anim_counter = 0;
	anim_frame = 0;

	while(1)
	{
		if (game_state == GAME_STATE_NORMAL)
		{
			if (mine_list == 0)
			{
				if (sfx_status_1 == 0 && sfx_status_2 == 0 && sfx_status_3 == 0)
				{
					game_state = GAME_STATE_HYPERSPACE;
					anim_counter = 0;
					anim_frame = 0;
				}
			}
			else if (player.state == PLAYER_STATE_REMOVED)
			{
				if (sfx_status_1 == 0 && sfx_status_2 == 0 && sfx_status_3 == 0)
				{
					if (player.extra_lives > 0)
					{
						player.extra_lives--;
						close_wave();
						init_player(&player, 0, 0, PLAYER_HEIGHT, PLAYER_WIDTH, 0, PLAYER_DRAW_SCALE, player_anim);
						generate_wave(0);
					}
				}
			}
			else
			{
				move_wave();
			}

			player_status = move_player(&player);
			enemy_status = move_mines(&player);
			move_ships(&player);
			move_bullets();

			if ((player_status & PLAYER_STATUS_THRUST) == PLAYER_STATUS_THRUST)
			{
				sfx_pointer_1 = (long unsigned int) (&thrust_snd_data);
				sfx_status_1 = 1;
			}

			if ((player_status & PLAYER_STATUS_FIRE) == PLAYER_STATUS_FIRE)
			{
				sfx_pointer_2 = (long unsigned int) (&bullet_snd_data);
				sfx_status_2 = 1;
			}

			if ((enemy_status & MINE_STATUS_EXPLODE) == MINE_STATUS_EXPLODE)
			{
				sfx_pointer_3 = (long unsigned int) (&explosion_snd_data);
				sfx_status_3 = 1;
			}

			Wait_Recal();

			if (sfx_status_1 == 1 || sfx_status_2 == 1 || sfx_status_3 == 1)
			{
				if (sfx_status_1 == 1)
				{
					ayfx_sound1();
				}

				if (sfx_status_2 == 1)
				{
					ayfx_sound2();
				}

				if (sfx_status_3 == 1)
				{
					ayfx_sound3();
				}

				Do_Sound();
			}

			Intensity_5F();
			reset_text();
			print_ulong(127, -12, player.score);

			Moveto_d(-127, -127);
			for (unsigned int i = 0; i < player.extra_lives; i++)
			{
				Dot_d(0, 4);
			}

			Intensity_7F();

			draw_animation(&player.anim);
			draw_mines();
			draw_ships();
			draw_bullets();

//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
			Reset0Ref();
			dp_VIA_t1_cnt_lo = OBJECT_MOVE_SCALE;

			Moveto_d(OBJECT_MIN_Y, OBJECT_MIN_X);

			Draw_Line_d(0, ((long) OBJECT_MAX_X - (long) OBJECT_MIN_X)/2L);
			Draw_Line_d(0, ((long) OBJECT_MAX_X - (long) OBJECT_MIN_X)/2L);

			Draw_Line_d(((long) OBJECT_MAX_Y - (long) OBJECT_MIN_Y)/2L, 0);
			Draw_Line_d(((long) OBJECT_MAX_Y - (long) OBJECT_MIN_Y)/2L, 0);

			Draw_Line_d(0, -((long) OBJECT_MAX_X - (long) OBJECT_MIN_X)/2L);
			Draw_Line_d(0, -((long) OBJECT_MAX_X - (long) OBJECT_MIN_X)/2L);

			Draw_Line_d(-((long) OBJECT_MAX_Y - (long) OBJECT_MIN_Y)/2L, 0);
			Draw_Line_d(-((long) OBJECT_MAX_Y - (long) OBJECT_MIN_Y)/2L, 0);
#endif
		}
		if (game_state == GAME_STATE_NEXT_LEVEL)
		{
			player.speed = 0;
			player.obj_pos[0] = player.obj_pos[1] = 0;
			generate_wave(1);
			game_state = GAME_STATE_NORMAL;
		}
		else if (game_state == GAME_STATE_HYPERSPACE)
		{
			game_seed += random();

			if (++anim_counter >= 2)
			{
				anim_counter = 0;
				if (++anim_frame > PLAYER_HYPERSPACE_NUM_FRAMES)
				{
					anim_frame = 0;
				}
			}

			check_buttons();
			if (button_1_4_pressed())
			{
				anim_counter = 0;
				anim_frame = 0;
				game_state = GAME_STATE_NEXT_LEVEL;
				random_long_seed(game_seed);
			}

			Wait_Recal();
			Moveto_d(0, 0);

			dp_VIA_t1_cnt_lo = 0x80;

			Intensity_3F();
			Draw_VLp((signed char *) player_hyperspace[anim_frame]);

			Intensity_7F();
			draw_animation(&player.anim);
		}
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
