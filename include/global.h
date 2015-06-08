#ifndef GLOBAL_H
#define GLOBAL_H
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include <stdio.h>

extern ei_widget_t *root_widget_window ;
extern ei_surface_t main_window_surface;
extern ei_surface_t picking_surface;
extern ei_widgetclass_t* list_class;
extern uint32_t widget_id_pick;

extern ei_color_t* id_to_color(ei_surface_t surface,uint32_t id);

#endif