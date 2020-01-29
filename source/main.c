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

#define MAX_MINES		10
#define MAX_SHIPS		2

#define SCALE 		100
#define DRAW_SCALE		0x10

const signed char tri_shape[]=
{
    -1,          0,  1*SCALE/2,
    -1, -1*SCALE/2, -1*SCALE/2,
    -1,  1*SCALE/2, -1*SCALE/2,
    -1,          0,  1*SCALE/2,
     1
};

static const signed char butterfly_shape[]=
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

void init_game(void)
{
	unsigned int i;

	for (i = 0; i < MAX_MINES; i++)
	{
		deinit_object(&mines[i].obj, &mine_list);
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

	init_player(&player, 0, 0, SCALE, SCALE, 0, DRAW_SCALE, tri_shape);

	//init_ship(&ships[0], 0, 0, SCALE, SCALE, 0, 0, DRAW_SCALE, butterfly_shape);
	init_mine(&mines[0], 80, 0, SCALE, SCALE, 0, DRAW_SCALE, tri_shape);
	init_ship(&ships[1], 0, 80, SCALE, SCALE, 240, 0, DRAW_SCALE, butterfly_shape);

	while(1)
	{
		move_player(&player);
		//if (++mines[0].world_angle == 255) mines[0].world_angle = 0;
		if (++ships[1].obj_angle == 255) ships[1].obj_angle = 0;
		//if (++ships[1].world_angle == 255) ships[1].world_angle = 0;
		mines[0].world_angle = player.angle;
		ships[1].world_angle = player.angle;
		move_mines();
		move_ships();
		Wait_Recal();
		draw_player(&player);
		draw_mines();
		draw_ships();
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
