#ifndef _OBJECT_H
#define _OBJECT_H

#include "element.h"

#define OBJECT_MOVE_SCALE	0xd0

#define OBJECT_MIN_Y		-80
#define OBJECT_MAX_Y		80
#define OBJECT_MIN_X		-74
#define OBJECT_MAX_X		74

#define OBJECT_WRAP_MIN_Y	-100
#define OBJECT_WRAP_MAX_Y	100
#define OBJECT_WRAP_MIN_X	-100
#define OBJECT_WRAP_MAX_X	100

struct object
{
	struct element elmnt;
	unsigned int active;
	signed int world_pos[2];
	signed int center_pos[2];
	signed int dim_2[2];
};

void init_object(
	struct object *obj,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	struct element **head
	);

void deinit_object(
	struct object *obj,
	struct element **head
	);

__INLINE unsigned int check_box_object(
	struct object *obj1,
	signed int obj2_y1,
	signed int obj2_x1,
	signed int obj2_y2,
	signed int obj2_x2
	)
{
	signed int obj1_center_x, obj1_center_y;
	signed int obj1_y1, obj1_x1, obj1_y2, obj1_x2;

	unsigned int result = 0;

	obj1_center_y = obj1->world_pos[0] + obj1->center_pos[0];
	obj1_center_x = obj1->world_pos[1] + obj1->center_pos[1];

	obj1_y1 = obj1_center_y + obj1->dim_2[0];
	obj1_x1 = obj1_center_x - obj1->dim_2[1];
	obj1_y2 = obj1_center_y - obj1->dim_2[0];
	obj1_x2 = obj1_center_x + obj1->dim_2[1];

	if (obj1_y1 < obj2_y2 && obj1_y2 > obj2_y1 && obj1_x1 < obj2_x2 && obj1_x2 > obj2_x1)
	{
		result = 1;
	}

	return result;
}

#endif
