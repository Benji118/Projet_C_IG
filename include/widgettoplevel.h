#ifndef EI_WIDGETTOPLEVEL_H
#define EI_WIDGETTOPLEVEL_H

#include "ei_widget.h"
#include "ei_types.h"

//Structure deffinissant la classe de widget toplevel
typedef struct ei_toplevel_t
{
	ei_widget_t widget;
	ei_color_t color;
	int border_size;
	char* title;
	ei_bool_t closable;
	ei_axis_set_t resizable;
	ei_size_t* min_size;
} ei_toplevel_t;

//Retourne un block memoire non initialisé de taille ei_toplevel_t
void* ei_toplevel_allocfunc();

//Desalloue un widget de classe ei_toplevel_t
void ei_toplevel_releasefunc(struct ei_widget_t* widget);

//Dessine un widget de classe ei_toplevel_t
void ei_toplevel_drawfunc(struct ei_widget_t*	widget,
	ei_surface_t		surface,
	ei_surface_t		pick_surface,
	ei_rect_t*		clipper);

//Parametres par default pour un widget de classe ei_toplevel_t
void ei_toplevel_setdefaultsfunc(struct ei_widget_t* widget);


void ei_toplevel_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

//Fonction de gestion d'evenement pour un widget de classe ei_toplevel_t
void ei_toplevel_handlefunc(struct ei_widget_t* widget,struct ei_event_t* event);


#endif
