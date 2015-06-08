#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "widgetbutton.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.h"

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
	/* Initialisation de la couche graphique */
	hw_init();

	/* registers all classes of widget and all geometry managers : */
	ei_frame_register_class();
	ei_button_register_class();
	ei_toplevel_register_class();

	/* Creation du widget racine de type frame */
	root_widget_window = ei_widget_create("frame",  NULL);
	ei_placer_params_t *params = malloc(sizeof(ei_placer_params_t));
	int *x = malloc(sizeof(int));
	int *y = malloc(sizeof(int));
	params->x = x;
	params->y = y;
	root_widget_window->placer_params = params;
	root_widget_window->placer_params->x = 0;
	root_widget_window->placer_params->y = 0;
	
	
	
	root_widget_window->requested_size = *main_window_size;

	//ei_surface_t picking_surface = hw_surface_create(main_window,main_window_size,EI_FALSE);

	main_window_surface = hw_create_window(main_window_size, fullscreen);

	
	hw_surface_lock(main_window_surface);
}

ei_widget_t *ei_app_root_widget()
{
	return root_widget_window;
}

ei_surface_t ei_app_root_surface()
{
	return main_window_surface;
}


void ei_app_free()
{
	/* ei_widget_t *root = ei_app_root_widget(); */
	/* ei_widget_destroy (root); */
	/* hw_quit(); */
}

void ei_app_quit_request()
{

}

static void affiche_widget_rec(ei_widget_t *widget, ei_rect_t* clipper)
{
	if (widget->placer_params != NULL)
		widget->wclass->drawfunc (widget, main_window_surface, picking_surface, clipper);
	
	if (widget->next_sibling != NULL)
		affiche_widget_rec(widget->next_sibling, clipper);
	
	if (widget->children_head != NULL)
		affiche_widget_rec(widget->children_head, widget->content_rect);
		
}

void ei_app_run()
{
	ei_bool_t quit_app = EI_FALSE;
	ei_event_t *event_cour = malloc(sizeof(ei_event_t));
	assert(event_cour != NULL);
	ei_widget_t  *widget_cour;
	ei_linked_rect_t *clipp_cour;
	ei_bool_t first_time = EI_TRUE;

	while (quit_app == EI_FALSE){
		/* if (ei_app_quit_request()){ */
		/* 	quit_app = EI_TRUE; */
		/* } */
		if (first_time == EI_TRUE){
			affiche_widget_rec(root_widget_window, NULL);
			hw_surface_unlock(ei_app_root_surface());
			hw_surface_update_rects(ei_app_root_surface(), NULL);
			first_time = EI_FALSE;
		} else {
			if (event_cour->type == ei_ev_mouse_buttondown){
				widget_cour = widget_pick(&(event_cour->param.mouse.where));
				if (strcmp(widget_cour->wclass->name, "button")){
					widget_cour = (ei_button_t *) widget_cour;
					widget_cour->relief = ei_relief_sunken;
					ei_app_invalidate_rect(&(widget_cour->screen_location));
				}
				while (clipp_cour != NULL){
					clipp_cour = list_rec;
					affiche_widget_rec(root_widget_window, clipp_cour->rect);
					hw_surface_unlock(ei_app_root_surface());
					hw_surface_update_rects(ei_app_root_surface(), NULL);
					clipp_cour = clipp_cour->next;
				}
			}
		}
		hw_event_wait_next(event_cour);
	}
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{
	
}
