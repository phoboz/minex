#ifndef _RANDOM_H
#define _RANDOM_H

void init_random(
	unsigned int s1,
	unsigned int s2,
	unsigned int s3,
	unsigned int x0
	);

unsigned int random(void);


unsigned long random_long(void);
void random_long_seed(
	unsigned long long seed
	);

#endif
