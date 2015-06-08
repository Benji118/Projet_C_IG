#include "ei_types.h"
#include "global.h"
#include "ei_widget.h"

void ei_event_set_active_widget(ei_widget_t* widget)
{
	if (widget==NULL) {
		active_widget=NULL;
	} else {
		active_widget=widget;
	}
}


ei_widget_t* ei_event_get_active_widget()
{
	return active_widget;
}


void ei_event_set_default_handle_func(ei_default_handle_func_t func)
{
}


ei_default_handle_func_t ei_event_get_default_handle_func()
{
}

ei_bool_t ei_default_handle_func_t (struct ei_event_t* event)
{
}
