#ifndef POLYGON_H
#define POLYGON_H
#include <stdbool.h>
#include "SDL_keysym.h"
#include <stdint.h>
#include "ei_types.h"


/**
*  Structure d'un cote pour TC dans la fonction ei_draw_polygon
*/
typedef struct ei_side_t {
  int                             ymax; // ordonnee maximale du cote
  int 							  dx,dy,e;
  int                             x_ymin; // abscisse du point d'ordonnee minimale du cote
  float                           rec_pente; //pente reciproque du cote
  struct ei_side_t*               next; //pointeur vers le cote suivant
} ei_side_t;

typedef struct TC_t {
	ei_side_t*** table;
	int nbre_cote;
}TC_t;

typedef struct cel_TCA cel_TCA;

struct cel_TCA {
	ei_side_t side;
	cel_TCA* next;
	int x_inter;
};

typedef struct TCA {
	cel_TCA* head;
}TCA;
//test d'egalité entre 2 cotés
bool  Comp_side (ei_side_t c1, ei_side_t c2);

//ajout en tete dans TCA
TCA *add_TCA(TCA *a, ei_side_t c);

//suppresion dans TCA
TCA *del_TCA (TCA *a, ei_side_t c);


TCA *sort_TCA (TCA *a);

void draw_pixel(ei_surface_t surface, const ei_color_t color, const ei_rect_t* clipper);
#endif
