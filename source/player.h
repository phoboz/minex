#ifndef _PLAYER_H
#define _PLAYER_H

#include "object.h"

struct player
{
	struct object obj;
	unsigned int angle;
};

void init_player(
	struct player *player,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int angle,
	unsigned int scale,
	const signed char *shape
	);

void move_player(
	struct player *player
	);

void draw_player(
	struct player *player
	);

#endif
