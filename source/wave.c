// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "mine.h"
#include "ship.h"
#include "bullet.h"
#include "imath.h"
#include "random.h"
#include "wave.h"

#define MAX_MINE_TYPES	12

// ---------------------------------------------------------------------------

extern struct player player;

struct mine_data
{
	unsigned int type;
	unsigned int size;
	long rarity;
};

static const struct mine_data md[MAX_MINE_TYPES]=
{
	// first level
	{	MINE_TYPE_DIRECTIONAL,						2,	MINE_COMMON		},
	{	MINE_TYPE_DIRECTIONAL,						1,	MINE_UNCOMMON		},
	{	MINE_TYPE_DIRECTIONAL,						0,	MINE_RARE			},

	// second level
	{	MINE_TYPE_MAGNETIC,						2,	MINE_COMMON		},
	{	MINE_TYPE_MAGNETIC,						1,	MINE_UNCOMMON		},
	{	MINE_TYPE_MAGNETIC,						0,	MINE_RARE			},

	// third level
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	2,	MINE_COMMON		},
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	1,	MINE_UNCOMMON		},
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	0,	MINE_RARE			},

	// forth level
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		2,	MINE_COMMON		},
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		1,	MINE_UNCOMMON		},
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		0,	MINE_RARE			}
};

//#define ENABLE_SHIP
#ifdef ENABLE_SHIP
#define SHIP_MODEL_SCALE 	16
#define SHIP_SIZE			20
#define SHIP_DRAW_SCALE	0x18

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
#endif

static signed char level = 0;
static unsigned long total_rarity = 0;

static const signed char lmod[] =
{
	100, 90, 80, 72, 64, 56, 50, 42, 35, 28, 20, 12, 4, 1
};

struct mine mines[MAX_MINES];
struct ship ships[MAX_SHIPS];
struct bullet bullets[MAX_BULLETS];

signed char max_mines(void)
{
	return min(level * 3, MAX_MINE_TYPES);
}

signed char mine_level(
	signed char index
	)
{
	return (index/3);
}

long mine_rarity(
	signed char index
	)
{
	long rarity = md[index].rarity;
	signed char level_diff = level - mine_level(index);

	return max(1, (rarity * lmod[min(13, level_diff)]) / 100);
}

signed char random_mine_type(void)
{
	long roll;
	signed char i;

	roll = (long) (random_long() % total_rarity) + 1;
	i = 0;

	while (roll > mine_rarity(i))
	{
		roll -= mine_rarity(i);
		i++;
	}
  
	return i;
}

static void init_minefield(void)
{
	unsigned int i;
	signed char mine_type;
	unsigned int type;
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

			mine_type = random_mine_type();
			type = md[mine_type].type;
			size = md[mine_type].size;

			init_mine(
				mine,
				(pos_y) ? (signed int) (random() % 100U) : -(signed int) (random() % 100U),
				(pos_x) ? (signed int) (random() % 100U) : -(signed int) (random() % 100U),
				type,
				size,
				15U + random() % 240U,
				&player
				);
		}
	}

#ifdef ENABLE_SHIP
	struct ship *ship = (struct ship *) ship_free_list;
	if (ship)
	{
		init_ship(ship, 0, 0, SHIP_SIZE, SHIP_SIZE, 0, &player, SHIP_DRAW_SCALE, mine_layer);
		ship->speed = 1;
	}
#endif
}

void clear_wave(void)
{
	unsigned int i;

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
}

void init_wave(void)
{
	signed char i, num_mines;

	level++;

	total_rarity = 0;
	num_mines = max_mines();

	for (i = 0; i < num_mines; i++)
	{
		total_rarity += (unsigned long) mine_rarity(i);
	}

	init_minefield();
}

// ***************************************************************************
// end of file
// ***************************************************************************
