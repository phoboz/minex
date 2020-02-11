// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "mine_data.h"

// ---------------------------------------------------------------------------

#define BLOW_UP MINE_MODEL_SCALE

static const signed char mine_0_1[]=
{	(signed char) 0x00, +0x03*BLOW_UP, -0x04*BLOW_UP, // move, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x03*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x03*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char mine_0_2[]=
{	(signed char) 0x00, +0x03*BLOW_UP/2, -0x04*BLOW_UP/2, // move, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, +0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, +0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, -0x03*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/2, -0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, -0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/2, -0x03*BLOW_UP/2, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char mine_0_3[]=
{	(signed char) 0x00, +0x03*BLOW_UP/3, -0x04*BLOW_UP/3, // move, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, +0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, +0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, -0x03*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/3, -0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, -0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/3, -0x03*BLOW_UP/3, // draw, y, x
	(signed char) 0x01 // endmarker 
};
const signed char * const mine_0[]=
{
	mine_0_3,
	mine_0_2,
	mine_0_1
};
const signed int mine_0_sizes[]=
{
	8,
	10,
	18
};

static const signed char mine_1_1[]=
{	(signed char) 0x00, +0x00*BLOW_UP, -0x04*BLOW_UP,  // move, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_1_2[]=
{	(signed char) 0x00, +0x00*BLOW_UP/2, -0x04*BLOW_UP/2,  // move, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/2, +0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, +0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, -0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, -0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/2, -0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, -0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_1_3[]=
{	(signed char) 0x00, +0x00*BLOW_UP/3, -0x04*BLOW_UP/3,  // move, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/3, +0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, +0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, -0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, -0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/3, -0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, -0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char * const mine_1[]=
{
	mine_1_3,
	mine_1_2,
	mine_1_1
};
const signed int mine_1_sizes[]=
{
	8,
	10,
	16
};

static const signed char mine_2_1[]=
{	(signed char) 0x00, +0x00*BLOW_UP, -0x03*BLOW_UP, // move, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, -0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, -0x04*BLOW_UP, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x01*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char mine_2_2[]=
{	(signed char) 0x00, +0x00*BLOW_UP/2, -0x03*BLOW_UP/2, // move, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, -0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, +0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, +0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/2, -0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/2, +0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, -0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, -0x04*BLOW_UP/2, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, +0x01*BLOW_UP/2, // draw, y, x
	(signed char) 0x01 // endmarker 
};
static const signed char mine_2_3[]=
{	(signed char) 0x00, +0x00*BLOW_UP/3, -0x03*BLOW_UP/3, // move, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, -0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, +0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, +0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/3, -0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x04*BLOW_UP/3, +0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, -0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, -0x04*BLOW_UP/3, // draw, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, +0x01*BLOW_UP/3, // draw, y, x
	(signed char) 0x01 // endmarker 
};
const signed char * const mine_2[]=
{
	mine_2_3,
	mine_2_2,
	mine_2_1
};
const signed int mine_2_sizes[]=
{
	10,
	12,
	22
};

static const signed char mine_3_1[]=
{	(signed char) 0x00, +0x00*BLOW_UP, -0x03*BLOW_UP,  // move, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x02*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_3_2[]=
{	(signed char) 0x00, +0x00*BLOW_UP/2, -0x03*BLOW_UP/2,  // move, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/2, +0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/2, +0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/2, +0x00*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/2, -0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, -0x02*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/2, +0x00*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/2, -0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_3_3[]=
{	(signed char) 0x00, +0x00*BLOW_UP/3, -0x03*BLOW_UP/3,  // move, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/3, +0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x03*BLOW_UP/3, +0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x01*BLOW_UP/3, +0x00*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x02*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/3, -0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, -0x02*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x01*BLOW_UP/3, +0x00*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x02*BLOW_UP/3, -0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char * const mine_3[]=
{
	mine_3_3,
	mine_3_2,
	mine_3_1
};
const signed int mine_3_sizes[]=
{
	6,
	8,
	12
};

static const signed char mine_explode_1[]=
{	(signed char) 0x00, -0x03*BLOW_UP, -0x03*BLOW_UP, // move to y, x
	(signed char) 0xFF, +0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, -0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP, +0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, +0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, -0x01*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP, -0x01*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP, -0x03*BLOW_UP,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP, +0x00*BLOW_UP,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_explode_2[]=
{	(signed char) 0x00, -0x03*BLOW_UP/2, -0x03*BLOW_UP/2, // move to y, x
	(signed char) 0xFF, +0x03*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/2, -0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP/2, +0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, +0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x00, -0x01*BLOW_UP/2, -0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, +0x00*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/2, +0x04*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP/2, -0x01*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/2, -0x03*BLOW_UP/2,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/2, +0x00*BLOW_UP/2,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
static const signed char mine_explode_3[]=
{	(signed char) 0x00, -0x03*BLOW_UP/3, -0x03*BLOW_UP/3, // move to y, x
	(signed char) 0xFF, +0x03*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/3, -0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP/3, +0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, +0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x00, -0x01*BLOW_UP/3, -0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, +0x00*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x00*BLOW_UP/3, +0x04*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x00, +0x03*BLOW_UP/3, -0x01*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, -0x03*BLOW_UP/3, -0x03*BLOW_UP/3,  // pattern, y, x
	(signed char) 0xFF, +0x04*BLOW_UP/3, +0x00*BLOW_UP/3,  // pattern, y, x
	(signed char) 0x01 // endmarker (high bit in pattern not set)
};
const signed char * const mine_explode[]=
{
	mine_explode_3,
	mine_explode_2,
	mine_explode_1
};

const signed char fireball[]=
{	(signed char) 0x00, +0x00*BLOW_UP, -0x02*BLOW_UP, // move, to y, x
	(signed char) 0xFF, +0x00*BLOW_UP, +0x04*BLOW_UP, // draw, y, x
	(signed char) 0x00, +0x01*BLOW_UP, -0x03*BLOW_UP, // mode, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, +0x02*BLOW_UP, // draw, y, x
	(signed char) 0x00, +0x03*BLOW_UP, -0x01*BLOW_UP, // mode, y, x
	(signed char) 0xFF, -0x04*BLOW_UP, +0x00*BLOW_UP, // draw, y, x
	(signed char) 0x00, +0x03*BLOW_UP, +0x01*BLOW_UP, // mode, y, x
	(signed char) 0xFF, -0x02*BLOW_UP, -0x02*BLOW_UP, // draw, y, x
	(signed char) 0x01 // endmarker 
};
// ***************************************************************************
// end of file
// ***************************************************************************
