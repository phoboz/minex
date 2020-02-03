#ifndef _PLAYER_H
#define _PLAYER_H

#include "object.h"

#define PLAYER_ACCELERATE_TRESHOLD	12
#define PLAYER_BRAKE_TRESHOLD		14
#define PLAYER_MAX_SPEED			3
#define PLAYER_FIRE_TRESHOLD		8
#define PLAYER_BULLET_SPEED			4

struct player
{
	struct object obj;
	signed int rel_pos[2];
	unsigned int update_view;
	unsigned int angle;
	unsigned int speed_counter;
	signed int speed;
	signed int up_vec[2];
	unsigned int fire_countdown;
	unsigned int scale;
	const signed char *shape;
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
