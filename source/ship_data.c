// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "ship_data.h"

// ---------------------------------------------------------------------------

#define BLOW_UP	SHIP_MODEL_SCALE

const signed char mine_layer[]=
{	(signed char) 0x00, +0x03*BLOW_UP, +0x01*BLOW_UP, // move, y, x
	(signed char) 0xFF, -0x06*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x07*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x07*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x06*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};

const signed char ship_explode[]=
{	(signed char) 0x00, +0x04*BLOW_UP, +0x00*BLOW_UP, // move, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};

// ***************************************************************************
// end of file
// ***************************************************************************
