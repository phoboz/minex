#ifndef _SHIP_H
#define _SHIP_H

#include "object.h"

#define SHIP_MAX_VECTORS	8

struct player;

struct ship
{
	struct object obj;
	signed int obj_pos[2];
	unsigned int obj_angle;
	unsigned int old_obj_angle;
	unsigned int world_angle;
	signed int front_vec[2];
	signed int speed;
	unsigned int scale;
	const signed char *shape;
	signed char obj_vlist[SHIP_MAX_VECTORS*3 + 1];
	signed char world_vlist[SHIP_MAX_VECTORS*3 + 1];
};

extern struct object *ship_list;
extern struct object *ship_free_list;

void init_ship(
	struct ship *ship,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int obj_angle,
	unsigned int world_angle,
	unsigned int scale,
	const signed char *shape
	);

void deinit_ship(
	struct ship *ship
	);

void move_ships(
	struct player *player
	);

void draw_ships(void);

#endif
