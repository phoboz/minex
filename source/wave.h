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

void init_wave();
void close_wave(void);
void reset_level_wave(void);

void generate_wave(
	unsigned int advance_level
	);

void move_wave(void);

unsigned long get_points_wave(
	struct mine *mine
	);

void announce_current_wave(void);

#endif
