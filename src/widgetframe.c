#include "widgetframe.h"
#include "ei_placer.h"
#include "ei_types.h"
#include <stdlib.h>
#include "ei_widgetclass.h"
#include "geometry.h"
#include <string.h>


void * ei_frame_allocfunc()
{
	void * memblock = malloc(sizeof(ei_frame_t));
	memset (memblock,0,sizeof(ei_frame_t));
	return memblock;
}

void ei_frame_releasefunc (struct ei_widget_t *widget)
{
	ei_frame_t* frame = (ei_frame_t*)widget;
	if ( frame != NULL )
		free(frame);
	return;
}

void ei_frame_drawfunc(struct ei_widget_t*	widget,
	ei_surface_t		surface,
	ei_surface_t		pick_surface,
	ei_rect_t*		clipper)
{
	ei_frame_t * frame = (ei_frame_t*) widget;

	draw_tool( surface, frame->widget.screen_location , 0.0, frame->border_size, frame->color, frame->relief, frame->clipper);

	//Offscreen
	draw_tool( pick_surface, frame->widget.screen_location, 0.0, 0.0, *(frame->widget.pick_color), 0, frame->clipper);

	//Cas du texte
	if(frame->texte != NULL)
	{
		int *w, *h;
		ei_point_t* text_pos = malloc(sizeof(ei_point_t));
		w = malloc(sizeof(int));
		h = malloc(sizeof(int));
		hw_text_compute_size(frame->texte,frame->font,w,h);
		if (frame->widget.screen_location.size.width > *w && 
			frame->widget.screen_location.size.height > *h)
		{
			text_pos->x = frame->widget.screen_location.top_left.x 
			+ (frame->widget.screen_location.size.width - *w)/2;
			text_pos->y = frame->widget.screen_location.top_left.y 
			+ (frame->widget.screen_location.size.height - *h)/2;

		}
			ei_draw_text(surface,text_pos,frame->texte,frame->font,
				frame->text_color,frame->clipper);
	}


	//Cas de l'image
	//ei_surface_t img_surface;
	if (frame->img !=NULL){
		//printf("img non NULL\n");
		//printf("%s\n",frame->img);
		//ei_surface_t img_surface = hw_image_load(frame->img,surface);
		ei_rect_t* rect_paste = frame->img_rect;
		if (rect_paste!=NULL) {
			printf("frame->img_rect :\n top (%d,%d)\n size : (%d,%d)\n",frame->img_rect->top_left.x,frame->img_rect->top_left.y,frame->img_rect->size.width,frame->img_rect->size.height);
			rect_paste->top_left = frame->img_pos;
		}
		hw_surface_lock(surface);
		ei_copy_surface(surface,rect_paste,*(frame->img),frame->img_rect,EI_FALSE);
		hw_surface_unlock(surface);
		hw_surface_update_rects(surface,NULL);
	}
}

void ei_frame_setdefaultsfunc	(struct ei_widget_t*	widget)
{
	widget->requested_size.width = 0;
	(widget->requested_size).height = 0;
	ei_frame_t* frame = (ei_frame_t*) widget;
	frame ->color = ei_default_background_color;
	frame->border_size=0;
	frame->relief=ei_relief_none;
	frame->img_rect=NULL;
	frame->text_anchor=ei_anc_center;
	frame->texte=NULL;
	frame->font = ei_default_font;
	ei_color_t *col = malloc(sizeof(ei_color_t));
	*col = ei_font_default_color;
	frame->text_color = col;
	frame->img=NULL;
	frame->img_anchor=ei_anc_center;
	ei_point_t img_point;
	img_point.x=0;
	img_point.y=0;
	frame->img_pos=img_point;

}

void ei_frame_geomnotifyfunc (struct ei_widget_t*	widget,
			      ei_rect_t		rect)
{	
	widget->screen_location = rect;
}

void ei_frame_handlefunc(struct ei_widget_t* widget,struct ei_event_t* event)
{

}
