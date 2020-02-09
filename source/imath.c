// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "imath.h"

// ---------------------------------------------------------------------------

unsigned int isqrt8(
	unsigned int num
	)
{
	unsigned int op  = num;
	unsigned int res = 0;
	unsigned int one = 0x40;

	while (one > op)
	{
		one >>= 2;
	}

	while (one != 0)
	{
		if (op >= res + one)
		{
			op -= res + one;
			res = (res >> 1) + one;
		}
		else
		{
			res >>= 1;
		}

		one >>= 2;
	}

	return res;
}

unsigned long isqrt16(
	unsigned long value
	)
{
	int i;
	unsigned long rem, root;

	rem  = 0;
	root = 0;

	// loop over the eight bits in the root
	for (i = 0; i < 8; i++)
	{
		// shift the root up by one bit
		root <<= 1;

		// move next two bits from the input into the remainder
		rem = ((rem << 2) + (value >> 14));
		value <<= 2;

		// test root is (2n + 1)
		root++;

		if (root <= rem)
		{
			// root not more than the remainder, so the new bit is one
			rem -= root;
			root++;
		}
		else
		{
			// root is greater than the remainder, so the new bit is zero
			root--;
		}
	}

	return (root >> 1);
}

// ***************************************************************************
// end of file
// ***************************************************************************
