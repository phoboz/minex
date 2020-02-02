#ifndef _BULLET_H
#define _BULLET_H

#define MAX_BULLETS		3

struct object;
struct player;

struct bullet
{
	unsigned int active;
	signed int world_pos[2];
	signed int speed;
};

extern struct bullet bullets[];

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int speed
	);

void deinit_bullet(
	struct bullet *bullet
	);

void fire_bullet(
	signed int y,
	signed int x,
	signed int speed
	);

void move_bullets(void);

unsigned int hit_object_bullet(
	struct bullet *bullet,
	struct object *obj
	);

void draw_bullets(void);

#endif
