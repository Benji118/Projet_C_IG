#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
{
	int *ir, *ig, *ib, *ia;
	ir=malloc(sizeof(*ir));
	ig=malloc(sizeof(*ig));
	ib=malloc(sizeof(*ib));
	ia=malloc(sizeof(*ia));
	assert(ir);
	assert(ig);
	assert(ib);
	assert(ia);
	hw_surface_get_channel_indices(surface,ir,ig,ib,ia);
	printf("ir : %i\nig : %i\nib : %i\nia : %i\n",*ir, *ig, *ib, *ia);
	uint32_t rgb = 1;
	return rgb;
}

void			ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) 
{	
}

void			ei_draw_polygon		(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper)
{
}

void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
}

void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
}

int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
}
