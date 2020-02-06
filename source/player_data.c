// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "player_data.h"

// ---------------------------------------------------------------------------

#define BLOW_UP PLAYER_MODEL_SCALE

static const signed char player_anim_0[]=
{	(signed char) 0x00, -0x01*BLOW_UP, +0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x00*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char player_anim_1[]=
{	(signed char) 0x00, -0x01*BLOW_UP, +0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x00*BLOW_UP, // draw, y, x
	(signed char) 0x00, -0x02*BLOW_UP, +0x02*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char player_anim_2[]=
{	(signed char) 0x00, -0x01*BLOW_UP, +0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x00*BLOW_UP, // draw, y, x
	(signed char) 0x00, -0x02*BLOW_UP, +0x02*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char player_anim_3[]=
{	(signed char) 0x00, -0x01*BLOW_UP, +0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x00*BLOW_UP, // draw, y, x
	(signed char) 0x00, -0x02*BLOW_UP, +0x02*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char player_anim_4[]=
{	(signed char) 0x00, -0x01*BLOW_UP, +0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x00*BLOW_UP, // draw, y, x
	(signed char) 0x00, -0x02*BLOW_UP, +0x02*BLOW_UP, // move to y, x
	(signed char) 0xFF, -0x04*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x03*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x03*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
const signed char * const player_anim[] =
{
	player_anim_0,
	player_anim_1,
	player_anim_2,
	player_anim_3,
	player_anim_4
};

// ***************************************************************************
// end of file
// ***************************************************************************
