#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {550,550};
	int		frame_x			= 0;
	int		frame_y			= 0;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	
	if (ei_app_root_widget()==NULL)
	{
		printf("NULL\n");
	}
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	char* name_img="misc/klimt.jpg";
	ei_surface_t img_surf=hw_image_load(name_img,ei_app_root_surface());
	ei_rect_t img_s_rect=hw_surface_get_rect(img_surf);
	//printf("size surf img : (%d,%d)\n",img_s_rect.size.width,img_s_rect.size.height);
	frame = ei_widget_create("frame", ei_app_root_widget());
	ei_frame_configure	(frame, &frame_size, &frame_color,
				 &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
				 &img_surf, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
