#include "widgetframe.h"
#include <stdlib.h>
#include "ei_widgetclass.h"
#include <string.h>

	ei_linked_point_t* frame_pos(ei_widget_t* frame,int border)
	{
		ei_linked_point_t pts [5];

		pts[0].point.x = frame->screen_location.top_left.x - border;
		pts[0].point.y = frame->screen_location.top_left.y - border;

		pts[1].point.x = pts[0].point.x + frame->screen_location.size.width + border;
		pts[1].point.y = pts[0].point.y;

		pts[2].point.x = pts[1].point.x;
		pts[2].point.y = pts[0].point.y + frame->screen_location.size.height + border;

		pts[3].point.x = pts[0].point.x;
		pts[3].point.y = pts[2].point.y;

		pts[4] = pts[0];

		for (int i = 0; i < 4; ++i)
		{
			pts[i].next = pts[i+1].next;
		}
		pts[4].next = NULL;

		return pts;
	}


	void * ei_frame_allocfunc()
	{
		void * memblock = malloc(sizeof(ei_frame_t));
		memset (memblock,0,sizeof(ei_frame_t));
		return memblock;
	}

	void ei_frame_releasefunc (struct ei_widget_t *widget)
	{
		ei_frame_t* frame = (ei_frame_t*)widget;
		if (frame == NULL)
		{
			return; 
		}
		else
		{
		free(frame);
	    }
	}

	void ei_frame_drawfunc(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
	{
		ei_frame_t* frame = (ei_frame_t*) widget;

		ei_color_t bckgrnd = { 0x33, 0x33, 0x33, 0xAA};
		ei_linked_point_t* border_shape = frame_pos(widget,frame->border_size) ; // (sans bordure)
		if (frame->border_size != 0)
		{
			ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
			ei_color_t      black       = { 0, 0, 0, 255 };
			ei_color_t      relief      = bckgrnd;


		//Relief frame
			ei_draw_polygon(surface,border_shape,relief,clipper);
			ei_draw_polyline(surface,border_shape,bckgrnd,clipper);

			if (frame->relief == ei_relief_raised)
			{
				ei_draw_polygon(surface,border_shape,white,clipper);
			}
			else if (frame->relief == ei_relief_sunken)
			{
				ei_draw_polygon(surface,border_shape,black,clipper);
			}
		}
		//Interieur frame
		free(border_shape);
		ei_linked_point_t* frame_shape = frame_pos(widget,0); // (avec bordure)
		ei_draw_polygon(surface,frame_shape,frame->color,clipper);
		ei_draw_polyline(surface,frame_shape,bckgrnd,clipper);

		//Offscreen
		ei_draw_polygon(pick_surface,frame_shape,*(frame->widget).pick_color,
			clipper);

		//Cas du texte
		if(frame->texte != NULL)
		{
			ei_draw_text(surface,frame->text_pos,frame->texte,frame->font,
				frame->text_color,frame->clipper);
		}

		//Cas de l'image
		ei_surface_t img_surface;
		if (frame->img !=NULL)
		{
			img_surface = hw_image_load(frame->img,surface);
			ei_rect_t* rect_paste = frame->img_rect;
			rect_paste->top_left = frame->img_pos;
			hw_surface_lock(img_surface);
			ei_copy_surface(surface,rect_paste,img_surface,
				frame->img_rect,EI_FALSE);
			hw_surface_unlock(img_surface);
		}
	}

	void ei_frame_setdefaultsfunc	(struct ei_widget_t*	widget)
	{
		(widget->requested_size).width = 0;
		(widget->requested_size).height = 0;
		ei_frame_t* frame = (ei_frame_t*) widget;
		frame ->color = ei_default_background_color;
		frame->border_size=0;
		frame->relief=ei_relief_none;
		frame->img_rect=NULL;
		frame->text_anchor=ei_anc_center;
		frame->texte=NULL;
		frame->font = ei_default_font;
		*(frame->text_color) = ei_font_default_color;
		frame->img=NULL;
		frame->img_anchor=ei_anc_center;

	}

	void ei_frame_geomnotifyfunc (struct ei_widget_t*	widget,
		                           ei_rect_t		rect)
	{	
		widget->screen_location = rect;
	}

