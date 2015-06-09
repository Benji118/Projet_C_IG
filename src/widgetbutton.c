#include "widgetbutton.h"
#include "geometry.h"
#include <stdlib.h>
#include <string.h>
#include "hw_interface.h"


void* ei_button_allocfunc()
{
	void * memblock = malloc(sizeof(ei_button_t));
	memset (memblock,0,sizeof(ei_button_t));
	return memblock;
}

void ei_button_releasefunc(struct ei_widget_t* widget)
{
	ei_button_t* button = (ei_button_t*)widget;
	if ( button != NULL )
		free(button);
	return;
}

void ei_button_drawfunc(struct ei_widget_t*	widget,
	ei_surface_t		surface,
	ei_surface_t		pick_surface,
	ei_rect_t*		clipper)
{
	ei_button_t* button = (ei_button_t*) widget;
	if (button->corner_radius!=NULL)
	{
		draw_tool(surface,button->widget.screen_location,(double)*button->corner_radius,button->border_size,button->color,button->relief,NULL);
		//Offscreen
		draw_tool(pick_surface,button->widget.screen_location,(double)*button->corner_radius,0,*(button->widget.pick_color),button->relief,clipper);

	}
	else
	{
		draw_tool(surface,button->widget.screen_location,0.0,button->border_size,button->color,button->relief,clipper);
		//Offscreen
		draw_tool(pick_surface,button->widget.screen_location,0,0,*(button->widget.pick_color),button->relief,clipper);

	}

	//Cas du texte
	if(button->texte != NULL){
		int *w, *h;
		ei_point_t* text_pos = malloc(sizeof(ei_point_t));
		w = malloc(sizeof(int));
		h = malloc(sizeof(int));
		hw_text_compute_size(button->texte,button->font,w,h);
		if (button->widget.screen_location.size.width > *w && 
			button->widget.screen_location.size.height > *h)
		{
			text_pos->x = button->widget.screen_location.top_left.x 
			+ (button->widget.screen_location.size.width - *w)/2;
			text_pos->y = button->widget.screen_location.top_left.y 
			+ (button->widget.screen_location.size.height - *h)/2;
			ei_draw_text(surface,text_pos,button->texte,button->font,
				     button->text_color,clipper);
		}
	}


	//Cas de l'image
	ei_surface_t img_surface;
	if (button->img !=NULL){
		img_surface = hw_image_load(button->img,surface);
		ei_rect_t* rect_paste = button->img_rect;
		rect_paste->top_left = button->img_pos;
		ei_copy_surface(surface,rect_paste,img_surface,
						button->img_rect,EI_FALSE);
	}
}

void ei_button_setdefaultsfunc(struct ei_widget_t* widget)
{
	(widget->requested_size).width = 0;
	(widget->requested_size).height = 0;
	ei_button_t* button = (ei_button_t*) widget;
	button ->color = ei_default_background_color;
	button->border_size=0;
	button->relief=ei_relief_none;
	button->img_rect=NULL;
	button->text_anchor=ei_anc_center;
	button->texte=NULL;
	button->font = ei_default_font;
	ei_color_t *col = malloc (sizeof(ei_color_t));
	*col = ei_font_default_color;
	button->img=NULL;
	button->img_anchor=ei_anc_center;
}

void ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect)
{
	widget->screen_location = rect;
}

void ei_button_handlefunc(struct ei_widget_t* widget,struct ei_event_t* event)
{
}