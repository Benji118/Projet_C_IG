#include "widgetbutton.h"
#include "geometry.h"
#include <stdlib.h>
#include <string.h>
#include "hw_interface.h"
#include "ei_event.h"
#include "global.h"
#include "ei_application.h"


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
		draw_tool(surface,button->widget.screen_location,
			(double)*button->corner_radius,button->border_size,button->color,
			button->relief,NULL);
		//Offscreen
		draw_tool(pick_surface,button->widget.screen_location,
			(double)*button->corner_radius,0,*(button->widget.pick_color),
			button->relief,clipper);

	}
	else
	{
		draw_tool(surface,button->widget.screen_location,0.0,
			button->border_size,button->color,button->relief,clipper);
		//Offscreen
		draw_tool(pick_surface,button->widget.screen_location,0,0,
			*(button->widget.pick_color),button->relief,clipper);

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
			
			//Decalage si bouton enfonce
			if (button->relief == ei_relief_sunken){
				text_pos->x = text_pos->x + (0.3)*button->border_size;
				text_pos->y = text_pos->y + (0.3)*button->border_size;
			}
			
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
	ei_linked_rect_t *clipp_cour;
	ei_button_t* button_cour = (ei_button_t*) widget;

	//Si c'est le bouton resize d'un toplevel
	if ( event->type == ei_ev_mouse_buttonup && strcmp(button_cour->widget.parent->wclass->name,"toplevel")==0 && button_cour->widget.parent->children_head == &(button_cour->widget)){
		button_cour->callback((ei_widget_t *)button_cour, event, NULL);
	}

	/* Si sortie du curseur de sa zone */
	/* Ou retour du curseur dans la zone */
	if ( event->type == ei_ev_mouse_move ){
		/* Si le bouton est enfoncé et que l'on n'es plus dessus */
		if ( button_cour->relief == ei_relief_sunken &&
		     (ei_widget_pick(&(event->param.mouse.where)) != widget)){
			button_cour->relief = ei_relief_raised;

			ei_app_invalidate_rect(&(button_cour->widget.screen_location));
			clipp_cour = list_rect;
			hw_surface_lock(ei_app_root_surface());
			while (clipp_cour != NULL)
			{
				affiche_widget_rec(root_widget_window, &(clipp_cour->rect));
				clipp_cour = clipp_cour->next;
			}
			ei_app_free_rect(&list_rect);
			hw_surface_unlock(ei_app_root_surface());
			hw_surface_update_rects(ei_app_root_surface(), NULL);
		}

		/* Si le bouton est relevé et qu'on revient dessus */
		if ( button_cour->relief == ei_relief_raised &&
		     (ei_widget_pick(&(event->param.mouse.where)) == widget)){
			button_cour->relief = ei_relief_sunken;

			ei_app_invalidate_rect(&(button_cour->widget.screen_location));
			clipp_cour = list_rect;
			hw_surface_lock(ei_app_root_surface());
			while (clipp_cour != NULL)
			{
				affiche_widget_rec(root_widget_window, &(clipp_cour->rect));
				clipp_cour = clipp_cour->next;
			}
			ei_app_free_rect(&list_rect);
			hw_surface_unlock(ei_app_root_surface());
			hw_surface_update_rects(ei_app_root_surface(), NULL);
		}
	}

	/* Si on appuie sur le bouton */
	if ( event->type == ei_ev_mouse_buttondown )
	{
		hw_surface_lock(ei_app_root_surface());
		if (button_cour->relief == ei_relief_raised) {
			button_cour->relief = ei_relief_sunken;
			ei_app_invalidate_rect(&(button_cour->widget.screen_location));
		}
		clipp_cour = list_rect;
		while (clipp_cour != NULL)
		{
			affiche_widget_rec(root_widget_window, &(clipp_cour->rect));
			clipp_cour = clipp_cour->next;
		}
		ei_app_free_rect(&list_rect);
		hw_surface_unlock(ei_app_root_surface());
		hw_surface_update_rects(ei_app_root_surface(), NULL); 
	}
	/* Si on releve le bouton de la souris */
	if (event->type==ei_ev_mouse_buttonup) {
		if (ei_event_get_active_widget()!=NULL) {
			hw_surface_lock(ei_app_root_surface());
			if (button_cour->relief == ei_relief_sunken) {
				button_cour->relief = ei_relief_raised;
			}
			ei_app_invalidate_rect(&(button_cour->widget.screen_location));
		}
		clipp_cour = list_rect;
		while (clipp_cour != NULL)
		{
			affiche_widget_rec(root_widget_window, &(clipp_cour->rect));
			clipp_cour = clipp_cour->next;
		}
		ei_app_free_rect(&list_rect);
		hw_surface_unlock(ei_app_root_surface());
		ei_event_set_active_widget(NULL);
		hw_surface_update_rects(ei_app_root_surface(), NULL);
		if (widget == ei_widget_pick(&(event->param.mouse.where))){
			/* Execution callback si on est revenu sur le bouton*/
			if (button_cour->callback != NULL)
				button_cour->callback((ei_widget_t *)button_cour, event, NULL);
		}
		
	}
	
}
