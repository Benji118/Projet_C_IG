#ifndef WIDGETBUTTON_H
#define WIDGETBUTTON_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_types.h"

//Structure deffinissant la classe de widget frame
typedef struct ei_button_t
{
	ei_widget_t widget;
	ei_color_t		color;
	double         		border_size;
	int*            corner_radius;
	ei_relief_t		relief;
	char *			texte;
	ei_point_t*              text_pos;
	ei_anchor_t		text_anchor;
	ei_font_t		font;
	ei_color_t*		text_color;
	char*                    img;
	ei_point_t	        img_pos;
	ei_anchor_t              img_anchor;
	ei_rect_t*		img_rect;
	ei_rect_t*		clipper;
	ei_callback_t   callback;
	void*           user_param;
}ei_button_t;

//Retourne un block memoire non initialisé de taille ei_button_t
void* ei_button_allocfunc();

//Desalloue un widget de classe ei_button_t
void ei_button_releasefunc(struct ei_widget_t* widget);

//Parametres par default pour un widget de classe ei_button_t
void ei_button_drawfunc(struct ei_widget_t*	widget,
	ei_surface_t		surface,
	ei_surface_t		pick_surface,
	ei_rect_t*		clipper);


void ei_button_setdefaultsfunc(struct ei_widget_t* widget);

void ei_button_geomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect);

//Fonction de gestion d'evenement pour un widget de classe ei_button_t
ei_bool_t ei_button_handlefunc(struct ei_widget_t* widget,struct ei_event_t* event);

#endif
