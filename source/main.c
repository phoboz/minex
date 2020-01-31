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
#include "controller.h"
#include "player.h"
#include "mine.h"
#include "ship.h"
#include "bullet.h"
#include "random.h"

#define MAX_MINES		10
#define MAX_SHIPS		2
#define MAX_BULLETS	3

#define SCALE 		24
#define DRAW_SCALE		0x10

#define BLOW_UP SCALE

const signed char mine_1_1[]=
{	(signed char) 0xFF, -0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};

const signed char mine_1_2[]=
{	(signed char) 0xFF, -0x02*BLOW_UP/2, +0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/2, +0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/2, -0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/2, +0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/2, -0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP/2, -0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, +0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, -0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};

const signed char mine_1_3[]=
{	(signed char) 0xFF, -0x02*BLOW_UP/3, +0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/3, +0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/3, -0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/3, +0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/3, -0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP/3, -0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, +0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, -0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};

const signed char * const mine_1[] =
{
	mine_1_3,
	mine_1_2,
	mine_1_1
};

const signed char player_ship[]=
{	(signed char) 0x00, +0x02*BLOW_UP, +0x00*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};

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

void init_game(void)
{
	unsigned int i;
	unsigned int size;

	for (i = 0; i < MAX_MINES; i++)
	{
		deinit_mine(&mines[i]);
	}

	for (i = 0; i < MAX_MINES; i++)
	{
		size = random() % 3;

		init_mine(
			&mines[i],
			(signed int) random() % 127 - 64,
			(signed int) random() % 127 - 64,
			((signed int) size + 1) * SCALE/4,
			((signed int) size + 1) * SCALE/4,
			MINE_TYPE_DIRECTIONAL,
			15U + (random() % 30U) * 8U,
			0,
			DRAW_SCALE,
			mine_1[size]
			);
	}

	for (i = 0; i < MAX_SHIPS; i++)
	{
		deinit_ship(&ships[i]);
	}

	for (i = 0; i < MAX_BULLETS; i++)
	{
		deinit_bullet(&bullets[i]);
	}
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
	enable_controller_1_x();
	enable_controller_1_y();
	disable_controller_2_x();
	disable_controller_2_y();

	init_random(35, 27, 3, 19);

	init_game();

	init_player(&player, 0, 0, SCALE/4, SCALE/4, 0, DRAW_SCALE, player_ship);
	init_ship(&ships[0], 0, 100, SCALE/4, SCALE/4, 0, 0, DRAW_SCALE, alien_ship);

ships[0].speed = 3;

	while(1)
	{
		move_player(&player);

		if (++ships[0].obj_angle == 64) ships[0].obj_angle = 0;

		move_mines(&player);
		move_ships(&player);
		move_bullets(&player);

		Wait_Recal();

		Intensity_7F();

		draw_player(&player);
		draw_mines();
		draw_ships();
		draw_bullets();
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
