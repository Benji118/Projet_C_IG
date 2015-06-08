#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ei_types.h"
#include "ei_draw.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "hw_interface.h"

ei_linked_point_t *arc(double rayon, ei_point_t centre, double depart, double arrivee);

ei_linked_point_t *rounded_frame(ei_rect_t rectangle,
				 double rayon,
				 bool top,
				 bool bottom);

void                    draw_tool        ( ei_surface_t                   surface,
					     const ei_rect_t                rectangle,
					     const double                   rayon,
					     const double                   edge,
					     const ei_color_t               color,
					     const ei_relief_t              relief,
					     const ei_rect_t*               clipper);
#endif
