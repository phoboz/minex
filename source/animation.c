// ***************************************************************************
// Vectrex C programming file template
// ***************************************************************************

#include <vectrex.h>
#include "animation.h"

// ---------------------------------------------------------------------------

void init_animation(
	struct animation *anim,
	signed int y,
	signed int x,
	signed int h,
	signed int w,
	unsigned int scale,
	const signed char * const *shapes
	)
{
	init_object(&anim->obj, y, x, h, w, 0);
	anim->scale	= scale;
	anim->shapes	= shapes;

	anim->base_frame	= 0;
	anim->frame		= 0;
}

void deinit_animation(
	struct animation *anim
	)
{
	deinit_object(&anim->obj, 0);
}

void draw_animation(
	struct animation *anim
	)
{
	if (anim->obj.active)
	{
		Reset0Ref();
		Moveto_d(0, 0);
//#define DEBUG_DRAW
#ifdef DEBUG_DRAW
		Moveto_d(-anim->obj.dim_2[0], -anim->obj.dim_2[1]);

		Draw_Line_d(0, anim->obj.dim_2[1] << 1);
		Draw_Line_d(anim->obj.dim_2[0] << 1, 0);
		Draw_Line_d(0, -anim->obj.dim_2[1] << 1);
		Draw_Line_d(-anim->obj.dim_2[0] << 1, 0);

		Moveto_d(anim->obj.dim_2[0], anim->obj.dim_2[1]);
#endif
		dp_VIA_t1_cnt_lo = anim->scale;
		Draw_VLp((signed int *) anim->shapes[anim->base_frame + anim->frame]);
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************
