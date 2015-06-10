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
ei_bool_t quit_app = EI_FALSE;

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
	picking_surface = hw_surface_create(ei_app_root_surface(),main_window_size,EI_FALSE);
	widget_id_pick = 0;
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
	ei_point_t origine = {0,0};
	ei_rect_t loc = {origine, *main_window_size};
	root_widget_window->screen_location = loc;
	/* root_widget_window->content_rect = &loc; */
	root_widget_window->content_rect = NULL;
	


	//hw_surface_lock(main_window_surface);
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
	quit_app = EI_TRUE;
}


void ei_app_run()
{
	ei_event_t *event_cour = malloc(sizeof(ei_event_t));
	assert(&event_cour != NULL);
	event_cour->type=ei_ev_none;

	/* ei_widget_t  *widget_cour =malloc(sizeof(ei_widget_t)); */
	/* widget_cour = NULL; */
	ei_event_set_active_widget(NULL);

	ei_default_handle_func_t traitant_defaut = ei_event_get_default_handle_func();
	ei_widgetclass_handlefunc_t traitant_interne;
	// ei_bool_t traite;

	hw_surface_lock(ei_app_root_surface());
	affiche_widget_rec(root_widget_window, NULL);
	hw_surface_unlock(ei_app_root_surface());
	hw_surface_update_rects(ei_app_root_surface(), NULL);

	while (quit_app == EI_FALSE)
	{
		if (event_cour->type == ei_ev_mouse_buttondown){
			ei_event_set_active_widget( ei_widget_pick(&(event_cour->param.mouse.where)) );
		}
		
		if (ei_event_get_active_widget() != NULL){
			traitant_interne = ei_event_get_active_widget()->wclass->handlefunc;
			traitant_interne(ei_event_get_active_widget(), event_cour);
		}
		hw_event_wait_next(event_cour);
		if (traitant_defaut != NULL)
			//On execute le traitant externe si il a été défini
			traitant_defaut(event_cour);
	}
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{
	ei_rect_t *copy=malloc(sizeof(ei_rect_t));
	*copy=*rect;
	if (list_rect==NULL) {
		list_rect=malloc(sizeof(ei_linked_rect_t));
		list_rect->rect=*copy;
		list_rect->next=NULL;
	} else {
		// Ajout en queue de liste
		ei_linked_rect_t *sent;
		sent=list_rect;
		while (sent->next!=NULL) {
			sent=sent->next;
		}
		sent->next=malloc(sizeof(ei_linked_rect_t));
		sent->next->rect=*copy;
		sent->next->next=NULL;
	}
}
