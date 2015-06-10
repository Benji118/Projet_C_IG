#ifndef WIDGETFRAME_H
#define WIDGETFRAME_H

#include "ei_event.h"
#include "SDL_keysym.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <stdio.h>
#include "ei_types.h"

//Structure deffinissant la classe de widget frame
typedef struct ei_frame_t {
	       ei_widget_t		widget; 
               ei_color_t		color;
               double         		border_size;
       	       ei_relief_t		relief;
	       char *			texte;
	       ei_anchor_t		text_anchor;
	       ei_font_t		font;
	       ei_color_t*		text_color;
	       ei_surface_t*            img;
	       ei_point_t	        img_pos;
	       ei_anchor_t              img_anchor;
	       ei_rect_t*		img_rect;
	       ei_rect_t*		clipper;
} ei_frame_t;

//Retourne un block memoire non initialisé de taille ei_frame_t
void * ei_frame_allocfunc();

//Desalloue un widget de classe ei_frame_t
void ei_frame_releasefunc(struct ei_widget_t * widget);


//Dessine un widget de classe ei_frame_t
void ei_frame_drawfunc(struct ei_widget_t*	widget,
		       ei_surface_t		surface,
		       ei_surface_t		pick_surface,
		       ei_rect_t*		clipper);

//Parametres par default pour un widget de classe ei_frame_t
void ei_frame_setdefaultsfunc	(struct ei_widget_t*	widget);

void ei_frame_geomnotifyfunc (struct ei_widget_t*	widget,
			      ei_rect_t		rect);

//Fonction de gestion d'evenement pour un widget de classe ei_frame_t
ei_bool_t ei_frame_handlefunc(struct ei_widget_t* widget,
			      struct ei_event_t* event);

#endif
