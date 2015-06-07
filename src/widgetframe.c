#include "widgetframe.h"
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

	//Frame : pas d'arrondi
	draw_button( surface, frame->widget.screen_location , 0.0, frame->border_size, frame->color, frame->relief, frame->clipper);

	//Offscreen
	draw_button( pick_surface, frame->widget.screen_location, 0.0, 0.0, frame->color, frame->relief, frame->clipper);

	//Cas du texte
	if(frame->texte != NULL){
		ei_draw_text(surface,frame->text_pos,frame->texte,frame->font,
			     frame->text_color,frame->clipper);
	}


	//Cas de l'image
	ei_surface_t img_surface;
	if (frame->img !=NULL){
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

