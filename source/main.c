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
#include "mine.h"

#define MAX_MINES		10

#define SCALE 		120
#define DRAW_SCALE		0x10

const signed char shape[]=
{
    -1,          0,  1*SCALE/2,
    -1, -1*SCALE/2, -1*SCALE/2,
    -1,  1*SCALE/2, -1*SCALE/2,
    -1,          0,  1*SCALE/2,
     1
};


struct mine mines[MAX_MINES];

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
	init_game();

	init_mine(&mines[0], 0, 0, SCALE, SCALE, 0, 0, DRAW_SCALE, shape);
	init_mine(&mines[1], 80, 0, SCALE, SCALE, 0, 0, DRAW_SCALE, shape);
	init_mine(&mines[2], 0, 80, SCALE, SCALE, 240, 0, DRAW_SCALE, shape);

	while(1)
	{
		if (++mines[1].world_angle > 254) mines[1].world_angle = 0;
		if (++mines[2].world_angle > 254) mines[2].world_angle = 0;
		if (++mines[2].obj_angle > 254) mines[2].obj_angle = 0;
		move_mines();
		Wait_Recal();
		draw_mines();
	};
	
	// if return value is <= 0, then a warm reset will be performed,
	// otherwise a cold reset will be performed
	return 0;
}

// ***************************************************************************
// end of file
// ***************************************************************************
