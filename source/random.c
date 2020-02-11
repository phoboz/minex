// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>

// ---------------------------------------------------------------------------

static unsigned int _x;
static unsigned int _a;
static unsigned int _b; 
static unsigned int _c;
 
void init_random(
	unsigned int s1,
	unsigned int s2,
	unsigned int s3,
	unsigned int x0
	)
{
	_x = x0;
	_a = s1;
	_b = s2;
	_c = s3;
	_x++;
	_a = (_a^_c^_x);
	_b = (_b+_a);
	_c = ((_c+(_b>>1))^_a);
}

unsigned int random(void)
{
	_x++;
	_a = (_a^_c^_x);
	_b = (_b+_a);
	_c = ((_c+(_b>>1))^_a);
	return _c;
}

static unsigned long long random_number = 1;
//32,7,5,3,2,1
#define MASK 0x80000057
unsigned long random_long(void)
{
	unsigned int i;

	for (i = 0; i < 16; i++)
	{
		if (random_number & 0x00000001)
		{
			random_number = (((random_number ^ MASK) >> 1) | 0x80000000);
		}
		else
		{
			random_number >>=1;
		}
	}

	return((unsigned long) random_number);
}

void random_long_seed(
	unsigned long long seed
	)
{
	if (seed == 0)
	{
		random_number = 1;
	}
	else
	{
		random_number = seed;
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
