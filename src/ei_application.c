#include "ei_types.h"
#include "ei_widget.h"
#include <stdlib.h>
#include <stdio.h>

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
	ei_surface_t main_window =NULL;
	ei_widget_t * root_widget_window = ei_widget_create("frame",root_widget_window);
	ei_surface_t picking_surface = hw_surface_create(main_window,main_window_size,EI_FALSE);

	hw_init();

	//registers all classes of widget and all geometry managers : TODO

    main_window = hw_create_window(main_window_size, fullscreen);

}

ei_widget_t *ei_app_root_widget()
{

}

ei_app_root_surface()
{

}


void ei_app_free()
{
	ei_widget_t *root = ei_app_root_widget();
	ei_widget_destroy (root);
	hw_quit();
}

ei_app_quit_request()
{

}

void ei_app_run()
{
	ei_bool_t quit_app = EI_FALSE;

	while (quit_app == EI_FALSE)
	{
		if (ei_app_quit_request())
		{
			quit_app = EI_TRUE;
		}
		//gestion de l'affichage : TODO
		//hw_event_wait_next();
		//gestion des evenements : TODO
	}
}

ei_app_invalidate_rect(ei_rect_t* rect)
{
	
}
