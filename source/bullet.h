#ifndef _BULLET_H
#define _BULLET_H

#include "object.h"

struct player;

struct bullet
{
	struct object obj;
	signed int obj_pos[2];
	signed int speed;
	signed int velocity[2];
	unsigned int world_angle;
};

extern struct object *bullet_list;
extern struct object *bullet_free_list;

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	signed int speed,
	signed int *velocity,
	unsigned int world_angle
	);

void deinit_bullet(
	struct bullet *bullet
	);

void move_bullets(
	struct player *player
	);

void draw_bullets(void);

#endif
