#ifndef _SHIP_H
#define _SHIP_H

#include "object.h"

#define SHIP_MAX_VECTORS		13

#define SHIP_STATE_IDLE		0
#define SHIP_STATE_ACTIVATE		1
#define SHIP_STATE_ACTIVE		2
#define SHIP_STATE_EXPLODE		3
#define SHIP_STATE_REMOVE		4

#define SHIP_DEFEAT_POINTS		100

#define SHIP_ACTIVATE_TRESHOLD	24
#define SHIP_EXPLODE_TRESHOLD	16

#define SHIP_STATUS_HIT				0x01
#define SHIP_STATUS_EXPLODE				0x02
#define SHIP_STATUS_PLAYER_EXTRA_LIFE	0x04
#define SHIP_STATUS_HIT_PLAYER			0x08

struct player;

struct ship
{
	struct object obj;
	signed int obj_pos[2];
	signed int rel_pos[2];
	unsigned int obj_angle;
	unsigned int old_obj_angle;
	unsigned int world_angle;
	signed int front_vec[2];
	signed int speed;
	unsigned int state_counter;
	unsigned int state;
	unsigned int num_hits;
	unsigned int scale;
	const signed char *shape;
	signed char obj_vlist[SHIP_MAX_VECTORS*3 + 1];
	signed char world_vlist[SHIP_MAX_VECTORS*3 + 1];
};

extern struct element *ship_list;
extern struct element *ship_free_list;

void init_ship(
	struct ship *ship,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int obj_angle,
	unsigned int num_hits,
	struct player *player,
	unsigned int scale,
	const signed char *shape
	);

void deinit_ship(
	struct ship *ship
	);

unsigned int move_ships(
	struct player *player
	);

void draw_ships(void);

#endif
