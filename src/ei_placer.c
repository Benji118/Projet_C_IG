#include "ei_types.h"
#include "ei_widget.c"
#include "ei_widgetclass.h"
#include "ei_placer.h"
#include "ei_application.h"

void			ei_place			(struct ei_widget_t*	widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height)
{
	if (anchor != NULL)
	{
		widget->placer_params->anchor = anchor;
		widget->placer_params->anchor_data = *anchor;
	}
	if (x != NULL)
	{
		widget->placer_params->x = x;
		widget->placer_params->x_data = *x;
	}
	if (y != NULL)
	{
		widget->placer_params->y = y;
		widget->placer_params->y_data = *y;
	}
	if (width != NULL)
	{
		widget->placer_params->w = width;
		widget->placer_params->w_data = *width;
	}
	if (height != NULL)
	{
		widget->placer_params->h = height;
		widget->placer_params->h_data = *height;
	}
	if (rel_x != NULL)
	{
		widget->placer_params->rx = rel_x;
		widget->placer_params->rx_data = *rel_x;
	}
	if (rel_y != NULL)
	{
		widget->placer_params->ry = rel_y;
		widget->placer_params->ry_data = *rel_y;
	}
	if (rel_width != NULL)
	{
		widget->placer_params->rw = rel_width;
		widget->placer_params->rw_data = *rel_width;
	}
	if (rel_height != NULL)
	{
		widget->placer_params->rh = rel_height;
		widget->placer_params->rh_data = *rel_height;
	}
}





void ei_placer_run(struct ei_widget_t* widget)
{
	if (widget->parent == NULL)
	{
		widget->screen_location.top_left.x = 0;
		widget->screen_location.top_left.y = 0;
		widget->screen_location.size = hw_surface_get_size(ei_app_root_widget());
	}
	widget->screen_location.size.width = widget->placer_params->rw_data
	*widget->parent->screen_location.size.width + widget->placer_params->w_data;
	widget->screen_location.size.height = widget->placer_params->rh_data
	*widget->parent->screen_location.size.height + widget->placer_params->h_data;

	//traitement anchor
	if (widget->placer_params->anchor_data == 1)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width*widget->placer_params->rx_data
	+ widget->screen_location.size.width/2;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height*widget->placer_params->ry_data
	+widget->placer_params->y_data - widget->screen_location.size.height/2;
	}
	if(widget->placer_params->anchor_data == 2)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width*widget->placer_params->rx_data
	+widget->placer_params->x_data - widget->screen_location.size.width/2;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height*widget->placer_params->ry_data
	+widget->placer_params->y_data;	
	}
	if (widget->placer_params->anchor_data == 3)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->screen_location.size.width*widget->placer_params->rx_data
	+ widget->placer_params->x_data - widget->screen_location.size.width;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height*widget->placer_params->ry_data
	+ widget->placer_params->y_data;
	}
	if (widget->placer_params->anchor_data == 4)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->screen_location.size.width*widget->placer_params->rx_data
	+ widget->placer_params->x_data - widget->screen_location.size.width;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height * widget->placer_params->ry_data
	+ widget->placer_params->y_data + widget->screen_location.size.height/2;
	}
	if (widget->placer_params->anchor_data == 5)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width*widget->placer_params->rx_data
	+ widget->placer_params->x_data - widget->screen_location.size.width;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height*widget->placer_params->ry_data
	+ widget->placer_params->y_data - widget->screen_location.size.height;
	}
	if (widget->placer_params->anchor_data == 6)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width * widget->placer_params->rx_data
	+ widget->placer_params->x_data - widget->screen_location.size.height/2;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height*widget->placer_params->y_data
	+ widget->placer_params->y_data - widget->screen_location.size.height;
	}
	if (widget->placer_params->anchor_data == 7)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width*widget->placer_params->rx_data
	+ widget->placer_params->x_data;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height * widget->placer_params->ry_data
	+ widget->placer_params->y_data - widget->screen_location.size.height;
	}
	if (widget->placer_params->anchor_data == 8)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width * widget->placer_params->rx_data
	+ widget->placer_params->x_data;
		widget->screen_location.top_left.y = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height + widget->placer_params->ry_data
	+ widget->placer_params->y_data - widget->screen_location.size.height/2;
	}
	if (widget->placer_params->anchor_data == 9)
	{
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.x
	+ widget->parent->screen_location.size.width * widget->placer_params->rx_data
	+ widget->placer_params->x_data;
		widget->screen_location.top_left.x = widget->parent->screen_location.top_left.y
	+ widget->parent->screen_location.size.height * widget->placer_params->y_data
	+ widget->placer_params->y_data;
	}
}




void ei_placer_forget(struct ei_widget_t* widget)
{
	if (widget != NULL && widget->placer_params != NULL)
	{
		free(widget->placer_params);
	}
}
