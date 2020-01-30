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
#define MAX_BULLETS	5

#define SCALE 		100
#define DRAW_SCALE		0x10

const signed char ship_shape[]=
{
    -1,          0,  1*SCALE/2,
    -1,  1*SCALE/2, -1*SCALE/2,
    -1, -1*SCALE/2, -1*SCALE/2,
    -1,          0,  1*SCALE/2,
     1
};

static const signed char mine_shape[]=
{
    -1, -1*SCALE/4,  1*SCALE/2,
    -1,  1*SCALE/2  ,        0,
    -1, -1*SCALE/4, -1*SCALE/2,
    -1,  1*SCALE/4, -1*SCALE/2,
    -1, -1*SCALE/2  ,        0,
    -1,  1*SCALE/4,  1*SCALE/2,
     1
};

struct player player;
struct mine mines[MAX_MINES];
struct ship ships[MAX_SHIPS];
struct bullet bullets[MAX_BULLETS];

void init_game(void)
{
	unsigned int i;

	for (i = 0; i < MAX_MINES; i++)
	{
		deinit_mine(&mines[i]);
	}

	for (i = 0; i < MAX_MINES; i++)
	{
		init_mine(
			&mines[i],
			(signed int) random() % 127 - 64,
			(signed int) random() % 127 - 64,
			SCALE/4,
			SCALE/4,
			MINE_TYPE_DIRECTIONAL,
			15U + (random() % 30U) * 8U,
			0,
			DRAW_SCALE,
			mine_shape
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

	init_game();

	init_player(&player, 0, 0, SCALE/4, SCALE/4, 0, DRAW_SCALE, ship_shape);

	//init_mine(&mines[0], 80, 0, SCALE/4, SCALE/4, 0, DRAW_SCALE, mine_shape);
	//init_mine(&mines[1], 0, -80, SCALE/4, SCALE/4, 0, DRAW_SCALE, mine_shape);
	//init_mine(&mines[2], 50, 50, SCALE/4, SCALE/4, 0, DRAW_SCALE, mine_shape);
	init_ship(&ships[0], 0, 80, SCALE/4, SCALE/4, 0, 0, DRAW_SCALE, ship_shape);

#if 0
mines[0].velocity[0] = -1;
mines[1].velocity[1] = 1;
mines[2].velocity[0] = mines[2].velocity[1] = 1;
#endif
ships[0].speed = 3;

	while(1)
	{
		move_player(&player);

		if (++ships[0].obj_angle == 64) ships[0].obj_angle = 0;

		move_mines(&player);
		move_ships(&player);
		move_bullets(&player);

		Wait_Recal();
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
