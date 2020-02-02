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
	signed int world_pos[2];
	signed int center_pos[2];
	signed int dim_2[2];
};

void init_object(
	struct object *obj,
	signed int y,
	signed int x,
	signed int h,
	signed int w
	);

void deinit_object(
	struct object *obj
	);

#endif
