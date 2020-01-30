#ifndef _MINE_H
#define _MINE_H

#include "object.h"

#define MINE_MAX_VECTORS	8

struct player;

struct mine
{
	struct object obj;
	signed int obj_pos[2];
	signed int velocity[2];
	unsigned int world_angle;
	unsigned int scale;
	const signed char *shape;
	signed char world_vlist[MINE_MAX_VECTORS*3 + 1];
};

extern struct object *mine_list;
extern struct object *mine_free_list;

void init_mine(
	struct mine *mine,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int world_angle,
	unsigned int scale,
	const signed char *shape
	);

void deinit_mine(
	struct mine *mine
	);

void move_mines(
	struct player *player
	);

void draw_mines(void);

#endif
