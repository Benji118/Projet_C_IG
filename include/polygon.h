#ifndef POLYGON_H
#define POLYGON_H
#include <stdbool.h>
#include "SDL_keysym.h"
#include <stdint.h>
#include "ei_draw.h"
#include "ei_types.h"


/**
*  Structure d'un cote pour TC dans la fonction ei_draw_polygon
*/
typedef struct ei_side_t {
	int                             ymax; // ordonnee maximale du cote
	int                             x_ymin; // abscisse du point d'ordonnee minimale du cote
	ei_point_t                      begin;
	ei_point_t                      end;
	struct ei_side_t*               next; //pointeur vers le cote suivant
} ei_side_t;

typedef struct cell_TCA cell_TCA;

struct cell_TCA {
	ei_side_t side;
	cell_TCA* next;
	int x_inter;
};

typedef struct TCA {
	cell_TCA* head;
}TCA;

// Test d'egalité entre 2 cotés
ei_bool_t comp_side (ei_side_t c1, int y);

// Création d'une TCA 
TCA *create_TCA();

// Ajout en tete dans TCA 
void add_TCA(TCA *a, ei_side_t c);

// Suppresion en queue dans TC
void del_TCA (TCA *a, int y);

// Fonction de tri
// A utiliser après chaque ajout dans TCA
// Décale la première cellule jusqu'à sa place
TCA *sort_TCA (TCA *a);

void draw_pixel (ei_surface_t surface,
		 uint32_t x,
		 uint32_t y,
		 const ei_color_t color,
		 uint32_t* pixel_ptr,
		 const ei_rect_t* clipper);
#endif 
