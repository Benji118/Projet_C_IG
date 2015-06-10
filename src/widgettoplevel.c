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
	ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;

	//dessin de la fenetre
	draw_tool(surface,toplevel->widget.screen_location,0.0,
		  toplevel->border_size,toplevel->color,ei_relief_raised,clipper);

	//Barre de titre
	ei_rect_t title_bar;
	title_bar.top_left    = toplevel->widget.screen_location.top_left;
	title_bar.size.height = 25;
	title_bar.size.width   = toplevel->widget.screen_location.size.width;
	ei_color_t black = {0xff, 0xff, 0xff, 0xff};
	
	draw_tool(surface, title_bar, 0.0, 0.0, black, 0, clipper);

	//Offscreen
	draw_tool(pick_surface,toplevel->widget.screen_location,0.0,
		  toplevel->border_size,*(toplevel->widget.pick_color),ei_relief_none,clipper);	

	if (toplevel->title != NULL)
	{
		ei_color_t black_title = {0,0,0,255};
		int *w,*h;
		w = malloc (sizeof(int));
		h = malloc (sizeof(int));
		ei_point_t* title_pos = malloc(sizeof(ei_point_t));
		hw_text_compute_size(toplevel->title,ei_default_font,w,h);
		if (toplevel->widget.screen_location.size.width > *w
			&& toplevel->widget.screen_location.size.height > *h)
		{
			title_pos->x = toplevel->widget.screen_location.top_left.x 
			+ (toplevel->widget.screen_location.size.width - *w)/2;
			title_pos->y = toplevel->widget.screen_location.top_left.y 
			+ (toplevel->widget.screen_location.size.height - *h)/2;
			ei_draw_text(surface,title_pos,toplevel->title,ei_default_font,&black_title,clipper);
		}
	}
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

void ei_toplevel_handlefunc(struct ei_widget_t* widget,struct ei_event_t* event)
{

}
