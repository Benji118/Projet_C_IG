#include "widgettoplevel.h"
#include "geometry.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include <stdlib.h>
#include <string.h>

void* ei_toplevel_allocfunc()
{
	void * memblock = malloc(sizeof(ei_toplevel_t));
	memset (memblock,0,sizeof(ei_toplevel_t));
	return memblock;
}

void ei_toplevel_releasefunc(ei_widget_t* widget)
{
	ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
	if (toplevel != NULL)
		free(toplevel);
}

void ei_toplevel_drawfunc (struct ei_widget_t* widget,
							ei_surface_t	surface,
							ei_surface_t	pick_surface,
							ei_rect_t*	clipper)
{

}

void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget)
{
	(widget->requested_size).width = 0;
	(widget->requested_size).height = 0;
	ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
	toplevel ->color = ei_default_background_color;
	toplevel->border_size=0;
	toplevel->title=NULL;
	toplevel->closable = EI_FALSE;
	toplevel->resizable = ei_axis_none;
	toplevel->min_size = malloc(sizeof(ei_rect_t));
	toplevel->min_size->width= 0;
	toplevel->min_size->height= 0;	
}

void ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect)
{
	widget->screen_location = rect;
}

void ei_toplevel_handlefunc(struct ei_widget_t* widget,struct ei_event* event)
{

}
