#ifndef POLYGON_H
#define POLYGON_H
#include <stdbool.h>
#include "SDL_keysym.h"
#include <stdint.h>
#include "ei_draw.h"
#include "ei_types.h"


/**
*  Structure d'un cote pour TC et TCA dans la fonction ei_draw_polygon
*/
typedef struct ei_side_t {
	int                             ymax; // ordonnee maximale du cote
	float                             x_ymin; // abscisse du point d'ordonnee minimale du cote
	float                           inv_pente;
	struct ei_side_t*               next; //pointeur vers le cote suivant
} ei_side_t;

typedef struct cell_TCA cell_TCA;

struct cell_TCA {
	ei_side_t side;
	cell_TCA* next;
	float x_inter;
};

typedef struct TCA {
	cell_TCA* head;
}TCA;


// Test d'egalité entre 2 cotés
ei_bool_t comp_side (ei_side_t c1, int y);

/**
 * \brief     Create a new list (TCA type).
 *
 * @return 		  The new list created by the function.
 */
TCA *create_TCA();


/**
 * \brief       Add a new side in the TCA list.
 *
 * @param       a         The list where to add the new side.
 * @param       c         The data to include in the new component of the list.
 */
void add_TCA(TCA *a, ei_side_t c);


/**
 * \brief       Delete all the elements in the list which have the y value in their structure
 *
 * @param       a         The list where to delete the element.
 * @param       y         The value that we are looking for in the list. Where a element has the same value the function delete the element.		        
 */
void del_TCA (TCA *a, int y);


/**
 * \brief       Sort a TCA list by looking the component x_inter in element structure
 *
 * @param       a         The list that we need to sort.
 *
 * @return 		  A sorted list   
 */
TCA *sort_TCA (TCA *a);


/**
 * \brief       Draws a pixel in the surface given in parameter. Respect the clipper parameter.
 *
 * @param       surface    The surface where to draw
 * @param       x,y        The coordonates of the pixel
 * @param       color      The color of the pixel
 * @param       pixel_ptr  A pointer on the origin of the surface
 * @param       clipper    The clipper parameter 
 */
void draw_pixel (ei_surface_t surface,
		 uint32_t x,
		 uint32_t y,
		 const ei_color_t color,
		 uint32_t* pixel_ptr,
		 const ei_rect_t* clipper);

uint32_t alpha_effect(ei_surface_t surface_source,
					  ei_surface_t surface_dest,
 					  uint32_t *pixel_source,
 					  uint32_t *pixel_dest);

/**
 * \brief       Frees the component of TC
 *
 * @param       begin       The beginning of a line of the table (ex : TC[k])	        
 */
void free_TC(ei_side_t *begin);

#endif 
