#ifndef _MINE_H
#define _MINE_H

#include "object.h"

#define MINE_MAX_VECTORS			9

#define MINE_TYPE_DIRECTIONAL		0x00
#define MINE_TYPE_MAGNETIC			0x10
#define MINE_TYPE_FIREBALL			0x20

#define MINE_SIZE_MASK				0x0f

#define MINE_STATE_IDLE			0
#define MINE_STATE_ACTIVATE			1
#define MINE_STATE_ACTIVE			2
#define MINE_STATE_EXPLODE			3
#define MINE_STATE_FIREBALL_IDLE	4
#define MINE_STATE_FIREBALL			5
#define MINE_STATE_REMOVE			6

#define MINE_FIREBALL_SPEED			4

#define MINE_TARGET_MIN_RANGE		8

#define MINE_ACTIVATE_TRESHOLD		7
#define MINE_TARGET_TRESHOLD		4
#define MINE_FIREBALL_IDLE_TRESHOLD	2
#define MINE_EXPLODE_TRESHOLD		8

#define MINE_STATUS_EXPLODE			0x01

struct player;

struct mine
{
	struct object obj;
	signed int obj_pos[2];
	signed int rel_pos[2];
	signed int velocity[2];
	unsigned int type_size;
	unsigned int state;
	unsigned int lo_counter;
	unsigned int hi_counter;
	unsigned int treshold;
	unsigned int world_angle;
	const signed char *shape;
	signed char world_vlist[MINE_MAX_VECTORS*3 + 1];
};

extern struct element *mine_list;
extern struct element *mine_free_list;

void init_mine(
	struct mine *mine,
	signed int y,
	signed int x,
	unsigned int type,
	unsigned int size,
	unsigned int treshold,
	struct player *player
	);

void deinit_mine(
	struct mine *mine
	);

unsigned int move_mines(
	struct player *player
	);

void draw_mines(void);

#endif
