#include "widgetframe.h"
#include <stdlib.h>
#include "ei_widgetclass.h"
#include <stdlib.h>



	void * ei_frame_allocfunc()
	{
		void * memblock = malloc(sizeof(ei_color_t)+4*sizeof(ei_widget_t)+
								 sizeof(ei_placer_params_t)+sizeof(ei_rect_t));
		return memblock;
	}

	void ei_frame_releasefunc (struct ei_widget_t *widget)
	{
	}

	void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
	{
	}

	void ei_frame_setdefaultsfunc	(struct ei_widget_t*	widget)
	{
	}

	void ei_frame_geomnotifyfunc (struct ei_widget_t*	widget,
		                           ei_rect_t		rect)
	{	
	}

