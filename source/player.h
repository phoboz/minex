#ifndef _PLAYER_H
#define _PLAYER_H

#include "animation.h"

#define PLAYER_ACCELERATE_TRESHOLD	12
#define PLAYER_BRAKE_TRESHOLD		14
#define PLAYER_MAX_SPEED			3
#define PLAYER_FIRE_TRESHOLD		8
#define PLAYER_BULLET_SPEED			4
#define PLAYER_ANIM_TRESHOLD		4
#define PLAYER_FLAME_NUM_FRAMES		2

#define PLAYER_STATE_NORMAL			0

struct player
{
	struct animation anim;
	signed int rel_pos[2];
	unsigned int update_view;
	unsigned int angle;
	unsigned int speed_counter;
	signed int speed;
	unsigned int anim_counter;
	signed int up_vec[2];
	unsigned int fire_countdown;
	unsigned int state;
	unsigned int scale;
};

void init_player(
	struct player *player,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int angle,
	unsigned int scale,
	const signed char * const *shapes
	);

void move_player(
	struct player *player
	);

void hit_player(
	struct player *player
	);

#endif
