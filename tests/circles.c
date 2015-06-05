#include "hw_interface.h"
#include "ei_utils.h"
#include "geometry.h"
#include "ei_types.h"
#include "ei_draw.h"

#include <stdlib.h>
#include <stdbool.h>

int ei_main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		color		= { 104, 33, 54, 255 };
	ei_rect_t*		clipper_ptr	= NULL;
//	ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
//	clipper_ptr		= &clipper;
	
	hw_init();
		
	main_window = hw_create_window(&win_size, EI_FALSE);
	
	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	//ei_fill		(main_window, &white, clipper_ptr);

	/* Draw circle. */
	ei_point_t centre = {400, 300};
	/* double rayon = 150.0; */
	/* double dep = 270.0; */
	/* double arr = 360.0; */

	/* Test arc */
	ei_linked_point_t *poly = malloc(sizeof(ei_linked_point_t));
	//poly = arc(rayon, centre, dep, arr);
	//ei_draw_polyline(main_window, poly, color, clipper_ptr);

	/* Test rounded_frame */
	ei_size_t taille = {250, 150};
	ei_rect_t rectangle = {centre, taille};
	ei_point_t centre2 = {100, 300};
	ei_rect_t rectangle2 = {centre2, taille};
	poly = rounded_frame(rectangle, 20.0, false, true);
	//ei_draw_polyline(main_window, poly, color, clipper_ptr);
	/* poly = rounded_frame(rectangle, 20.0, true, false); */
	/* ei_draw_polyline(main_window, poly, color, clipper_ptr); */

	/* Test bouton */
	draw_button        (main_window, rectangle, 20.0, 5.0, color, 0, clipper_ptr);
	draw_button        (main_window, rectangle2, 20.0, 5.0, color, ei_relief_sunken, clipper_ptr);
	
	
	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);
	
	/* Wait for a character on command line. */
	getchar();
	
	hw_quit();
	return (EXIT_SUCCESS);
}
