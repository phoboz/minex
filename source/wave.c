// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "imath.h"
#include "random.h"
#include "ship_data.h"
#include "wave.h"

#define MAX_MINE_TYPES	12

#define MINE_UNIQUE		 0L
#define MINE_VERY_RARE		 1L
#define MINE_RARE			 5L
#define MINE_UNCOMMON		10L
#define MINE_COMMON		20L
#define MINE_VERY_COMMON	30L

#define MAX_IDLE_TIMES		9

// ---------------------------------------------------------------------------

extern struct player player;

struct mine_data
{
	unsigned int type;
	unsigned int size;
	unsigned long points;
	unsigned long rarity;
};

static const struct mine_data md[MAX_MINE_TYPES]=
{
	// first level
	{	MINE_TYPE_DIRECTIONAL,						2,	100,		MINE_VERY_COMMON	},
	{	MINE_TYPE_DIRECTIONAL,						1,	120,		MINE_COMMON		},
	{	MINE_TYPE_DIRECTIONAL,						0,	150,		MINE_UNCOMMON		},

	// second level
	{	MINE_TYPE_MAGNETIC,						2,	200,		MINE_COMMON		},
	{	MINE_TYPE_MAGNETIC,						1,	220,		MINE_UNCOMMON		},
	{	MINE_TYPE_MAGNETIC,						0,	250,		MINE_RARE			},

	// third level
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	2,	300,		MINE_COMMON		},
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	1,	320,		MINE_UNCOMMON		},
	{	MINE_TYPE_DIRECTIONAL | MINE_TYPE_FIREBALL,	0,	350,		MINE_RARE			},

	// forth level
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		2,	400,		MINE_COMMON		},
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		1,	420,		MINE_UNCOMMON		},
	{	MINE_TYPE_MAGNETIC | MINE_TYPE_FIREBALL,		0,	250,		MINE_RARE			}
};

static unsigned int level;
static unsigned int ship_activated;
static unsigned long total_rarity;

static const unsigned int lmod[] =
{
	100, 90, 80, 72, 64, 56, 50, 42, 35, 28, 20, 12, 4, 1
};

static const unsigned int idle_times[MAX_IDLE_TIMES] =
{
	240, 220, 200, 180, 160, 140, 120, 100, 80
};

struct mine mines[MAX_MINES];
struct ship ships[MAX_SHIPS];
struct bullet bullets[MAX_BULLETS];

static unsigned int max_mine_types(void)
{
	return min(level * 3, MAX_MINE_TYPES);
}

static unsigned int mine_level(
	unsigned int index
	)
{
	return (index / 3);
}

static unsigned long mine_rarity(
	unsigned int index
	)
{
	unsigned long rarity = md[index].rarity;
	unsigned int level_diff = level - mine_level(index);

	return max(1, (rarity * lmod[min(13, level_diff)]) / 100);
}

static unsigned int random_mine_type(void)
{
	unsigned long roll;
	unsigned int i;

	roll = random_long() % total_rarity + 1;
	i = 0;

	while (roll > mine_rarity(i))
	{
		roll -= mine_rarity(i);
		i++;
	}
  
	return i;
}

static unsigned int get_idle_time(void)
{
	unsigned int idle_time;

	if (level < MAX_IDLE_TIMES)
	{
		idle_time = idle_times[level];
	}
	else
	{
		idle_time = idle_times[MAX_IDLE_TIMES - 1];
	}

	return idle_time;
}

static void init_minefield(void)
{
	unsigned int i;
	unsigned int mine_type;
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
				15U + random() % get_idle_time(),
				&player
				);
		}
	}
}

void clear_wave(void)
{
	unsigned int i;

	ship_activated = 0;
	total_rarity = 0;

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
	level = 0;
	clear_wave();
}

void generate_wave(
	signed int level_advance
	)
{
	unsigned int i, num;

	if (level_advance)
	{
		level += (unsigned int) level_advance;
	}
	ship_activated = 0;
	total_rarity = 0;

	num = max_mine_types();

	for (i = 0; i < num; i++)
	{
		total_rarity += (unsigned long) mine_rarity(i);
	}

	init_minefield();
}

void move_wave(void)
{
	static unsigned int ship_counter;
	static unsigned int ship_treshold;
	static unsigned int ship_angle;

	struct ship *ship;
	struct mine *mine;
	unsigned int mine_type;

	if (ship_activated)
	{
		ship = (struct ship *) ship_list;
		if (ship)
		{
			if (++ship_counter >= ship_treshold)
			{
				ship_counter = 0;

				mine = (struct mine *) mine_free_list;
				if (mine)
				{
					mine_type = random_mine_type();
					init_mine(
						mine,
						ship->obj_pos[0],
						ship->obj_pos[1],
						md[mine_type].type,
						md[mine_type].size,
						5U + random() % get_idle_time(),
						&player
						);
				}

				ship_treshold = 50 + (random() % 100);
				ship_angle = 8 * (random () % 8);
			}
			else
			{
				if (ship->obj_angle < ship_angle)
				{
					ship->obj_angle++;
				}
				else if (ship->obj_angle > ship_angle)
				{
					ship->obj_angle--;
				}
			}
		}
	}
	else
	{
		if (num_mines <= 3)
		{
			ship = (struct ship *) ship_free_list;
			if (ship)
			{
				init_ship(ship, -100, -100, SHIP_SIZE, SHIP_SIZE, 0, &player, SHIP_DRAW_SCALE, mine_layer);
				ship->speed = 1;
				ship_activated = 1;
				ship_counter = 0;
				ship_treshold = 0;
			}
		}
	}
}

unsigned long get_points_wave(
	struct mine *mine
	)
{
	unsigned int type, size;
	unsigned int i, num;
	unsigned long result = 0;

	size = mine->type_size & MINE_SIZE_MASK;
	type = mine->type_size & MINE_TYPE_MASK;

	num = max_mine_types();

	for (i = 0; i < num; i++)
	{
		if (md[i].type == type && md[i].size == size)
		{
			result = md[i].points;
			break;
		}
	}

	return result;
}

// ***************************************************************************
// end of file
// ***************************************************************************
