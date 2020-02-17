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
#include "draw.h"
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

#define GAME_STATE_NORMAL			0
#define GAME_STATE_NEXT_LEVEL		1
#define GAME_STATE_TITLE			2
#define GAME_STATE_HYPERSPACE		3
#define GAME_STATE_OVER			4

#define PLAYER_NUM_EXTRA_LIVES		3

#define GAME_FLAGS_ANNOUNCE_WAVE	0x01
#define GAME_FLAGS_FLASH_SHIP		0x02

#define GAME_START_INTENSITY		0x3F
#define GAME_MAX_INTENSITY			0xFF
#define GAME_DELTA_INTENSITY		0x0F

#define GAME_ANIM_TRESHOLD			2
#define GAME_ANNOUNCE_WAVE_TRESHOLD	50
#define GAME_HIT_FLASH_TRESHOLD		6

extern const unsigned int bullet_snd_data[];
extern const unsigned int hit_snd_data[];
extern const unsigned int explosion_snd_data[];
extern const unsigned int thrust_snd_data[];

struct player player;

static const char game_over_text[]	= "GAME OVER\x80";
static const char title_text[]	= "PRESS FIRE TO START\x80";

static unsigned int game_state;
static unsigned long game_seed;
static unsigned int game_flags;
static unsigned int game_counter;
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
	unsigned int ship_status;

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
	player.extra_lives = PLAYER_NUM_EXTRA_LIVES;

	game_state = GAME_STATE_TITLE;
	game_flags = GAME_FLAGS_ANNOUNCE_WAVE;
	game_counter = 0;
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
					game_counter = 0;
					anim_frame = 0;
					Vec_Music_Flag = 1;
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
						game_flags |= GAME_FLAGS_ANNOUNCE_WAVE;
						init_player(&player, 0, 0, PLAYER_HEIGHT, PLAYER_WIDTH, 0, PLAYER_DRAW_SCALE, player_anim);
						generate_wave(0);
					}
					else
					{
						game_state = GAME_STATE_OVER;
					}
				}
			}
			else
			{
				move_wave();
			}

			player_status = move_player(&player);
			enemy_status = move_mines(&player);
			ship_status = move_ships(&player);
			move_bullets();

			if ((game_flags & GAME_FLAGS_ANNOUNCE_WAVE) == GAME_FLAGS_ANNOUNCE_WAVE)
			{
				if (++game_counter >= GAME_ANNOUNCE_WAVE_TRESHOLD)
				{
					game_counter = 0;
					game_flags &= ~GAME_FLAGS_ANNOUNCE_WAVE;
				}
			}

			if ((game_flags & GAME_FLAGS_FLASH_SHIP) == GAME_FLAGS_FLASH_SHIP)
			{
				if (++game_counter >= GAME_HIT_FLASH_TRESHOLD)
				{
					game_counter = 0;
					game_flags &= ~GAME_FLAGS_FLASH_SHIP;
				}
			}
			else if ((ship_status & SHIP_STATUS_HIT) == SHIP_STATUS_HIT)
			{
				game_flags |= GAME_FLAGS_FLASH_SHIP;
				game_counter = 0;
			}

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
			else if ((ship_status & SHIP_STATUS_HIT) == SHIP_STATUS_HIT)
			{
				sfx_pointer_3 = (long unsigned int) (&hit_snd_data);
				sfx_status_3 = 1;
			}
			else if ((ship_status & SHIP_STATUS_EXPLODE) == SHIP_STATUS_EXPLODE)
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

			if ((game_flags & GAME_FLAGS_ANNOUNCE_WAVE) == GAME_FLAGS_ANNOUNCE_WAVE)
			{
				announce_current_wave();
			}

			Moveto_d(-127, -127);
			for (unsigned int i = 0; i < player.extra_lives; i++)
			{
				Dot_d(0, 4);
			}

			Intensity_7F();

			draw_animation(&player.anim);
			draw_mines();

			if ((game_flags & GAME_FLAGS_FLASH_SHIP) == GAME_FLAGS_FLASH_SHIP)
			{
				Intensity_3F();
				draw_ships();
				Intensity_7F();
			}
			else
			{
				draw_ships();
			}

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
			/* Regenerate player to avoid strange velocity vector */
			init_player(&player, 0, 0, PLAYER_HEIGHT, PLAYER_WIDTH, 0, PLAYER_DRAW_SCALE, player_anim);
			generate_wave(1);
			game_state = GAME_STATE_NORMAL;
		}
		else if (game_state == GAME_STATE_TITLE)
		{
			game_seed += random();

			if (++game_counter >= GAME_ANIM_TRESHOLD)
			{
				game_counter = 0;
				anim_frame += GAME_DELTA_INTENSITY;
				if (anim_frame >= (GAME_MAX_INTENSITY - GAME_START_INTENSITY))
				{
					anim_frame = 0;
				}
			}

			check_buttons();
			if (button_1_4_pressed())
			{
				game_counter = 0;
				anim_frame = 0;
				game_state = GAME_STATE_NEXT_LEVEL;
				game_flags |= GAME_FLAGS_ANNOUNCE_WAVE;
				random_long_seed(game_seed);
			}

			Wait_Recal();

			Intensity_5F();
			draw_synced_list_c((signed char *) minex_logo, 24, 0, 0x80, 0x40);

			Intensity_a(GAME_START_INTENSITY + anim_frame);
			reset_text();
			Print_Str_d(-24, -56, (char *) title_text);

		}
		else if (game_state == GAME_STATE_HYPERSPACE)
		{
			if (++game_counter >= GAME_ANIM_TRESHOLD)
			{
				game_counter = 0;
				if (++anim_frame > PLAYER_HYPERSPACE_NUM_FRAMES)
				{
					anim_frame = 0;
				}
			}

			if (Vec_Music_Flag)
			{
				DP_to_C8();
				Init_Music_chk(&Vec_Music_0);
			}
			else
			{
				game_state = GAME_STATE_NEXT_LEVEL;
				game_flags |= GAME_FLAGS_ANNOUNCE_WAVE;
			}

			Wait_Recal();
			Moveto_d(0, 0);

			Do_Sound();

			Intensity_5F();
			dp_VIA_t1_cnt_lo = 0x80;
			Draw_VLp((signed char *) player_hyperspace[anim_frame]);

			Reset0Ref();
			Moveto_d(0, 0);

			Intensity_7F();
			dp_VIA_t1_cnt_lo = 0x20;
			Draw_VLp((signed char *) player_behind);
		}
		else if (game_state == GAME_STATE_OVER)
		{
			check_buttons();
			if (button_1_4_pressed())
			{
				close_wave();
				player.score = 0;
				player.extra_lives = PLAYER_NUM_EXTRA_LIVES;
				reset_level_wave();
				game_state = GAME_STATE_TITLE;
				game_counter = 0;
				anim_frame = 0;
			}

			move_mines(&player);
			move_ships(&player);

			Wait_Recal();
			Moveto_d(0, 0);

			Intensity_5F();
			reset_text();
			print_ulong(127, -12, player.score);

			reset_text();
			Print_Str_d(0, -24, (char *) game_over_text);

			Intensity_7F();
			draw_mines();
			draw_ships();
		}
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
