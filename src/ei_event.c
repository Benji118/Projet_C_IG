#include "ei_types.h"
#include "global.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_event.h"

static ei_widget_t* active_widget = NULL;
static ei_default_handle_func_t handlefunc = NULL;

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
	handlefunc = func;
}


ei_default_handle_func_t ei_event_get_default_handle_func()
{
	return handlefunc;
}
