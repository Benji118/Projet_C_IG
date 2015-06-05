#ifndef WIDGET_H
#define WIDGET_H

#include "SDL_keysym.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <stdio.h>
#include "ei_types.h"


	typedef struct ei_frame_t {
		ei_widget_t			widget;
		ei_size_t			size;
		ei_color_t			color;
		int         		border_size;
		ei_relief_t			relief;
		ei_rect_t*		    img_rect;
		ei_anchor_t			text_anchor;
		char *				texte;
		ei_font_t			font;
		ei_color_t			text_color;
		ei_rect_t			clipper;
		char*               img;
		ei_point_t			img_pos;
		ei_anchor_t         img_anchor;
	} ei_frame_t;

	void * ei_frame_allocfunc();

	void ei_frame_releasefunc(struct ei_widget_t * widget);

	void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

	void ei_frame_setdefaultsfunc	(struct ei_widget_t*	widget);

	void ei_frame_geomnotifyfunc (struct ei_widget_t*	widget,
		                           ei_rect_t		rect);

	ei_bool_t ei_frame_handlefunc(struct ei_widget_t* widget,
		                           struct ei_event* event);

	#endif