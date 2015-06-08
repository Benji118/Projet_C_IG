#include "global.h"

ei_widget_t *root_widget_window ;
ei_surface_t main_window_surface;
ei_surface_t picking_surface;
ei_widgetclass_t* list_class;
uint32_t widget_id_pick = 0;

ei_color_t* id_to_color(ei_surface_t surface,uint32_t id)
{
	int ir,ig,ib,ia;
	hw_surface_get_channel_indices(surface,&ir,&ig,&ib,&ia);
	char Pr,Pg,Pb,Pa;
	Pr = (id >> ir*8) & 0xFF;
	Pg = (id >> ig*8) & 0xFF;
	Pb = (id >> ib*8) & 0xFF;
	Pa = (id >> ia*8) & 0xFF;
	ei_color_t res = {Pr,Pg,Pb,Pa};
	return &res;
}
