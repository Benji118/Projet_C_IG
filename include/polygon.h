#ifndef POLYGON_H
#define POLYGON_H

#include "SDL_keysym.h"
#include <stdint.h>
#include "ei_types.h"


/**
* @brief Structure d'un cote pour TC dans la fonction ei_draw_polygon
*/
typedef struct ei_side_t {
  int                             ymax; ///< ordonnee maximale du cote
  int                             x_ymin; ///< abscisse du point d'ordonnee minimale du cote
  float                           rec_pente; ///<pente reciproque du cote
  struct ei_side_t*               next; ///<pointeur vers le cote suivant
} ei_side_t;

typedef struct cel_TCA cel_TCA;

struct cel_TCA {
	ei_side_t side;
	cel_TCA* next;
};

typedef struct TCA {
	cel_TCA* head;
}TCA;

TCA *add_TCA(TCA *a, ei_side_t c);

TCA *del_TCA (TCA *a, ei_side_t c);

#endif