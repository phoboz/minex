#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "object.h"

struct animation
{
	struct object obj;
	unsigned int base_frame;
	unsigned int frame;
	unsigned int scale;
	const signed char * const *shapes;
};

void init_animation(
	struct animation *anim,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int scale,
	const signed char * const *shapes
	);

void deinit_animation(
	struct animation *anim
	);

void draw_animation(
	struct animation *anim
	);

#endif
