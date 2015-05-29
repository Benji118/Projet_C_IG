#include "hw_interface.h"
#include "ei_draw.h"

void *	ei_widgetclass_allocfunc_t		()
{
}


void	ei_widgetclass_releasefunc_t		(struct ei_widget_t*	widget)
{
}


 void	ei_widgetclass_drawfunc_t		(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
 {
 }


void	ei_widgetclass_setdefaultsfunc_t	(struct ei_widget_t*	widget)
{
}


void	ei_widgetclass_geomnotifyfunc_t	(struct ei_widget_t*	widget,
							 ei_rect_t		rect)
{
}


ei_bool_t ei_widgetclass_handlefunc_t(struct ei_widget_t*	widget,
						 struct ei_event_t*	event)
{
}



void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{
}


ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name)
{
}


void			ei_frame_register_class 	()
{
}


void			ei_button_register_class 	()
{
}


void			ei_toplevel_register_class 	()
{
}
