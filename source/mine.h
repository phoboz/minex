#ifndef _MINE_H
#define _MINE_H

#include "object.h"

#define MINE_MAX_VECTORS	8

struct mine
{
	struct object obj;
	signed char world_pos[2];
	unsigned int obj_angle;
	unsigned int world_angle;
	signed char obj_vlist[MINE_MAX_VECTORS*3 + 1];
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
	unsigned int obj_angle,
	unsigned int world_angle,
	unsigned int scale,
	const signed char *shape
	);

void deinit_mine(
	struct mine *mine
	);

void move_mines(void);
void draw_mines(void);


#endif
