#include "global.h"
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"

ei_widget_t *root_widget_window ;
ei_surface_t main_window_surface;
ei_surface_t picking_surface;
ei_widgetclass_t* list_class;
ei_linked_rect_t *list_rect;
uint32_t widget_id_pick ;
int x_last_click;
int y_last_click;


ei_color_t id_to_color(ei_surface_t surface,uint32_t id)
{
	int *ir, *ig, *ib, *ia;
	ir = malloc(sizeof(int));
	ig = malloc(sizeof(int));
	ib = malloc(sizeof(int));
	ia = malloc(sizeof(int));

	hw_surface_get_channel_indices(surface,ir,ig,ib,ia);

	char Pr,Pg,Pb,Pa;
	Pr = (id >> *ir*8) & 0xFF;
	Pg = (id >> *ig*8) & 0xFF;
	Pb = (id >> *ib*8) & 0xFF;
	Pa = (id >> *ia*8) & 0xFF;
	ei_color_t res = {Pr,Pg,Pb,Pa};
	return res;
}

void ei_app_free_head(ei_linked_rect_t **list){
	ei_linked_rect_t *supp = (*list);
	(*list) = (*list)->next;
	free(supp);
}

void ei_app_free_rect(ei_linked_rect_t** list_rect) {
	while (*list_rect != NULL)
		ei_app_free_head(list_rect);
}

void affiche_widget_rec(ei_widget_t *widget, ei_rect_t* clipper)
{
	if (widget->placer_params != NULL)
		widget->wclass->drawfunc (widget, main_window_surface, picking_surface, clipper);
	
	if (widget->next_sibling != NULL)
		affiche_widget_rec(widget->next_sibling, clipper);
	
	if (widget->children_head != NULL)
		affiche_widget_rec(widget->children_head, &(widget->screen_location));
}
