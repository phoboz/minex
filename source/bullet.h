#ifndef _BULLET_H
#define _BULLET_H

#include "object.h"

struct player;

struct bullet
{
	struct element elmnt;
	signed int world_pos[2];
	signed int speed;
};

extern struct element *bullet_list;
extern struct element *bullet_free_list;

void init_bullet(
	struct bullet *bullet,
	signed int y,
	signed int x,
	signed int speed
	);

void deinit_bullet(
	struct bullet *bullet
	);

void move_bullets(void);
void draw_bullets(void);

__INLINE unsigned int hit_object_bullet(
	struct bullet *obj1,
	struct object *obj2
	)
{
	signed int obj1_y, obj1_x;
	signed int obj2_y1, obj2_x1, obj2_y2, obj2_x2;
	unsigned int result = 0;

	obj1_y = obj1->world_pos[0];
	obj1_x = obj1->world_pos[1];

	obj2_y1 = obj2->world_pos[0] + obj2->dim_2[0];
	obj2_x1 = obj2->world_pos[1] - obj2->dim_2[1];
	obj2_y2 = obj2->world_pos[0] - obj2->dim_2[0];
	obj2_x2 = obj2->world_pos[1] + obj2->dim_2[1];

	if (obj1_y > obj2_y1 && obj1_y < obj2_y2 && obj1_x > obj2_x1 && obj1_x < obj2_x2)
	{
		result = 1;
	}

	return result;
}

__INLINE unsigned int hit_dim_object_bullet(
	struct bullet *obj1,
	struct object *obj2,
	signed int h,
	signed int w
	)
{
	signed int obj1_y, obj1_x;
	signed int obj2_y1, obj2_x1, obj2_y2, obj2_x2;
	unsigned int result = 0;

	obj1_y = obj1->world_pos[0];
	obj1_x = obj1->world_pos[1];

	obj2_y1 = obj2->world_pos[0] + h;
	obj2_x1 = obj2->world_pos[1] - w;
	obj2_y2 = obj2->world_pos[0] - h;
	obj2_x2 = obj2->world_pos[1] + w;

	if (obj1_y > obj2_y1 && obj1_y < obj2_y2 && obj1_x > obj2_x1 && obj1_x < obj2_x2)
	{
		result = 1;
	}

	return result;
}

#endif
