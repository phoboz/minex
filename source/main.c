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
#include "bullet.h"
#include "random.h"

#define MAX_MINES		10
#define MAX_SHIPS		1
#define MAX_BULLETS	3

#define SHIP_MODEL_SCALE 	24
#define SHIP_SIZE			20
#define SHIP_DRAW_SCALE	0x10

#define BLOW_UP	SHIP_MODEL_SCALE

extern const unsigned int bullet_snd_data[];
extern const unsigned int explosion_snd_data[];

const signed char alien_ship[]=
{	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};

struct player player;
struct mine mines[MAX_MINES];
struct ship ships[MAX_SHIPS];
struct bullet bullets[MAX_BULLETS];

void init_level(void)
{
	unsigned int i;
	unsigned int size;
	unsigned int pos_y, pos_x;

	struct mine *mine;
	for (i = 0; i < MAX_MINES; i++)
	{
		mine = (struct mine *) mine_free_list;
		if (mine)
		{
			pos_y = random() % 2;
			pos_x = random() % 2;
			size = random() % 3;

			init_mine(
				mine,
				(pos_y) ? (signed int) (random() % 100U) : -(signed int) (random() % 100U),
				(pos_x) ? (signed int) (random() % 100U) : -(signed int) (random() % 100U),
				mine_1_sizes[size],
				mine_1_sizes[size],
				MINE_TYPE_DIRECTIONAL,
				15U + random() % 240U,
				&player,
				MINE_DRAW_SCALE,
				mine_1[size]
				);
		}
	}

#if 0
	struct ship *ship = (struct ship *) ship_free_list;
	if (ship)
	{
		init_ship(ship, 0, 100, SHIP_SIZE, SHIP_SIZE, 0, &player, SHIP_DRAW_SCALE, alien_ship);
		ship->speed = 3;
	}
#endif
}

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
	unsigned int i;
	unsigned int player_status;
	unsigned int enemy_status;

	enable_controller_1_x();
	enable_controller_1_y();
	disable_controller_2_x();
	disable_controller_2_y();

	init_random(35, 27, 3, 19);

	for (i = 0; i < MAX_MINES; i++)
	{
		mines[i].obj.active = 0;
		give_element(&mines[i].obj.elmnt, &mine_free_list);
	}

	for (i = 0; i < MAX_SHIPS; i++)
	{
		ships[i].obj.active = 0;
		give_element(&ships[i].obj.elmnt, &ship_free_list);
	}

	for (i = 0; i < MAX_BULLETS; i++)
	{
		give_element(&bullets[i].elmnt, &bullet_free_list);
	}

	init_player(&player, 0, 0, PLAYER_HEIGHT, PLAYER_WIDTH, 0, PLAYER_DRAW_SCALE, player_anim);

	while(1)
	{
		if (mine_list == 0)
		{
			player.rel_pos[0] = player.rel_pos[1];
			player.speed = 0;
			init_level();
		}

		player_status = move_player(&player);

		//if (++ships[0].obj_angle == 64) ships[0].obj_angle = 0;

		enemy_status = move_mines(&player);
		move_ships(&player);
		move_bullets();

		if ((player_status & PLAYER_STATUS_FIRE) == PLAYER_STATUS_FIRE)
		{
			sfx_pointer_1 = (long unsigned int) (&bullet_snd_data);
			sfx_status_1 = 1;
		}

		if ((enemy_status & MINE_STATUS_EXPLODE) == MINE_STATUS_EXPLODE)
		{
			sfx_pointer_2 = (long unsigned int) (&explosion_snd_data);
			sfx_status_2 = 1;
		}

		Wait_Recal();

		if (sfx_status_1 == 1 || sfx_status_2 == 1)
		{
			if (sfx_status_1 == 1)
			{
				ayfx_sound1();
			}

			if (sfx_status_2 == 1)
			{
				ayfx_sound2();
			}
			Do_Sound();
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
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
