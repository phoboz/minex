#ifndef _OBJECT_H
#define _OBJECT_H

#define OBJECT_MOVE_SCALE	0x80

#define OBJECT_MIN_Y		-100
#define OBJECT_MAX_Y		100
#define OBJECT_MIN_X		-100
#define OBJECT_MAX_X		100

struct object
{
	unsigned int active;
	signed int center_pos[2];
	signed int rel_pos[2];
	signed int world_pos[2];
	signed int dim_2[2];

	struct object *prev, *next;
};

void  give_object(
	struct object *obj,
	struct object **head
	);

void take_object(
	struct object *obj,
	struct object **head
	);

void init_object(
	struct object *obj,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	struct object **head
	);

void deinit_object(
	struct object *obj,
	struct object **head
	);

__INLINE unsigned int hit_particle_object(
	struct object *obj1,
	struct object *obj2
	)
{
	signed int obj1_y, obj1_x;
	signed int obj2_y1, obj2_x1, obj2_y2, obj2_x2;

	unsigned int result = 0;

	obj1_y = obj1->world_pos[0];
	obj1_x = obj1->world_pos[1];

	obj2_y1 = obj2->world_pos[0] + obj2->center_pos[0] + obj2->dim_2[0];
	obj2_x1 = obj2->world_pos[1] + obj2->center_pos[1] - obj2->dim_2[1];
	obj2_y2 = obj2->world_pos[0] + obj2->center_pos[0] - obj2->dim_2[0];
	obj2_x2 = obj2->world_pos[1] + obj2->center_pos[1] + obj2->dim_2[1];

	if (obj1_y > obj2_y1 && obj1_y < obj2_y2 && obj1_x > obj2_x1 && obj1_x < obj2_x2)
	{
		result = 1;
	}

	return result;
}

#endif
