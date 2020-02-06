// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "mine_data.h"

// ---------------------------------------------------------------------------

#define BLOW_UP MINE_MODEL_SCALE

static const signed char mine_1_1[]=
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
static const signed char mine_1_2[]=
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
static const signed char mine_1_3[]=
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
const signed int mine_1_sizes[] =
{
	8,
	10,
	16
};

// ***************************************************************************
// end of file
// ***************************************************************************
