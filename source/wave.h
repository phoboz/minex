#ifndef _WAVE_H
#define _WAVE_H

#define MAX_MINES		10
#define MAX_SHIPS		1
#define MAX_BULLETS	3

#define MINE_UNIQUE		   0L
#define MINE_VERY_RARE		 100L
#define MINE_RARE			 500L
#define MINE_UNCOMMON		1000L
#define MINE_COMMON		4000L

extern struct mine mines[];
extern struct ship ships[];
extern struct bullet bullets[];

void clear_wave(void);
void init_wave();

#endif
