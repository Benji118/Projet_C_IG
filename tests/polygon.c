#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"

int ei_main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(150,110);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_color_t		red		= { 0xff, 0x00, 0x00, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
	//ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
	//clipper_ptr		= &clipper;
	
	hw_init();
		
	main_window = hw_create_window(&win_size, EI_FALSE);
	
	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	ei_linked_point_t	pts[8];
	pts[0].point.x = 20; pts[0].point.y = 30; pts[0].next = &(pts[1]);
	pts[1].point.x = 90; pts[1].point.y = 10; pts[1].next = &(pts[2]);
	pts[2].point.x = 130; pts[2].point.y = 50; pts[2].next = &(pts[3]);
	pts[3].point.x = 120; pts[3].point.y = 100; pts[3].next = &(pts[4]);
	pts[4].point.x = 70; pts[4].point.y = 70; pts[4].next = &(pts[5]);
	pts[5].point.x = 20; pts[5].point.y = 90; pts[5].next = &(pts[6]);
	pts[6].point.x = 20; pts[6].point.y = 30; pts[6].next = NULL;
	
	ei_draw_polygon(main_window,pts,red,clipper_ptr);
	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);
	
	/* Wait for a character on command line. */
	getchar();
	
	hw_quit();
	return (EXIT_SUCCESS);
}
