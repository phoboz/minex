#ifndef _WAVE_H
#define _WAVE_H

#include "mine.h"
#include "ship.h"
#include "bullet.h"

#define MAX_MINES		10
#define MAX_SHIPS		1
#define MAX_BULLETS	3

extern struct mine mines[];
extern struct ship ships[];
extern struct bullet bullets[];

void clear_wave(void);
void init_wave();

unsigned long get_points_wave(
	struct mine *mine
	);

#endif
