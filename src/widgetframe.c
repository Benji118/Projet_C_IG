#include "widgetframe.h"



	void * ei_frame_allocfunc()
	{
		ei_widgetclass frame;
		void * memblock = malloc(sizeof(ei_color_t)+4*sizeof(ei_widget_t)+
								 sizeof(placer_params)+sizeof(ei_rect_t));
		frame.allocfunc = &frame_allocfunc;
		return memblock;
	}

	void ei_frame_releasefunc ()
	{
	
	}

	void ei_frame_drawfunc()
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

	ei_bool_t ei_frame_handlefunc(struct ei_widget_t* widget,
		                           struct ei_event* event)
	{
		
	}

