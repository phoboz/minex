#ifndef _BULLET_H
#define _BULLET_H

#include "object.h"

struct player;

struct bullet
{
	struct object obj;
	signed int speed;
};

extern struct object *bullet_list;
extern struct object *bullet_free_list;

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	signed int speed
	);

void deinit_bullet(
	struct bullet *bullet
	);

void move_bullets(void);
void draw_bullets(void);

#endif
