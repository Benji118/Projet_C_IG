#ifndef GLOBAL_H
#define GLOBAL_H
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include <stdio.h>

extern ei_widget_t *root_widget_window ; //widget global de la racine
extern ei_surface_t main_window_surface; //surface globale de dessin
extern ei_surface_t picking_surface; //surface globale de picking
extern ei_widgetclass_t* list_class; //liste globale d'enregistrement des classes de widgets
extern ei_linked_rect_t *list_rect;//liste de rectangles à mettre à jour pour le dessin
extern uint32_t widget_id_pick; /*id de picking (parametre global incremente
 à chaque creation de widget)*/
extern int x_last_click; /*abscisse du pointeur lors d'un enfoncement du bouton de la souris
					pour le deplacement des toplevels*/
extern int y_last_click; /*ordonnee du pointeur lors d'un enfoncement du bouton de la souris
					pour le deplacement des toplevels*/

//Prend en argument le contenu d'un pixel et renvoi sa couleur
ei_color_t id_to_color(ei_surface_t surface,uint32_t id);

//Detruit la tete d'une liste chainee de rectangle passé en argument
void ei_app_free_head(ei_linked_rect_t **list);

//Detruit une liste chainee de rectangle passé en argument
void ei_app_free_rect(ei_linked_rect_t** list_rect);

//Affichage recursif d'un widget et de sa descendance
void affiche_widget_rec(ei_widget_t *widget, ei_rect_t* clipper);

#endif
