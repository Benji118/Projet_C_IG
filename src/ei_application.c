#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_application.h"
#include "widgetbutton.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "global.h"

ei_surface_t ei_app_root_surface();

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
	/* Initialisation de la couche graphique */
	hw_init();

	/* registers all classes of widget and all geometry managers : */
	ei_frame_register_class();
	ei_button_register_class();
	ei_toplevel_register_class();
	
	/*Creations de la surface_root et de la surface de picking*/
	main_window_surface = hw_create_window(main_window_size, fullscreen);
	picking_surface = hw_surface_create(ei_app_root_surface(),
		main_window_size,EI_FALSE);

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
	//ei_event_t event_cour;
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
				printf("bouton");
				widget_cour = ei_widget_pick(&(event_cour->param.mouse.where));
				if (widget_cour != NULL){
					if (strcmp(widget_cour->wclass->name, "button")){
						ei_button_t *button_cour = (ei_button_t *) widget_cour;
						button_cour->relief = ei_relief_sunken;
						ei_app_invalidate_rect(&(button_cour->widget.screen_location));
					}
					clipp_cour = list_rect;
					while (clipp_cour != NULL){
						affiche_widget_rec(root_widget_window, &(clipp_cour->rect));
						hw_surface_unlock(ei_app_root_surface());
						hw_surface_update_rects(ei_app_root_surface(), NULL);
						clipp_cour = clipp_cour->next;
					}
				}
			}
		}
		//printf("event_wait");
		hw_event_wait_next(event_cour);
		//printf("event_pass");
	}
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{
	ei_rect_t *copy=malloc(sizeof(ei_rect_t));
	*copy=*rect;
	if (list_rect==NULL) {
		printf("first\n");
		list_rect=malloc(sizeof(ei_linked_rect_t));
		list_rect->rect=*copy;
		list_rect->next=NULL;
	} else {
		// Ajout en queue de liste
		ei_linked_rect_t *sent;
		sent=list_rect;
		while (sent->next!=NULL) {
			printf("next\n");
			sent=sent->next;
		}
		sent->next=malloc(sizeof(ei_linked_rect_t));
		sent->next->rect=*copy;
		sent->next->next=NULL;
	}
}
