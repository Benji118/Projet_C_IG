#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "widgetframe.h"
#include "ei_placer.h"
#include <stdlib.h>

// void		ei_callback_t	(ei_widget_t*		widget,
// 				 struct ei_event_t*	event,
// 				 void*			user_param)
// {
// }


ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent)
{
	ei_widget_t *tmp1;
	ei_widgetclass_t *class = ei_widgetclass_from_name(class_name);
	if (class == NULL)
	{
		return NULL;
		exit(0);
	}
	void * memsize = class->allocfunc();
	ei_frame_t* new_widget = malloc(sizeof(*memsize));
	if (parent != NULL)
	{
	new_widget->widget.parent = parent;
	if (parent->children_head == NULL)
	{
		parent->children_head = &(new_widget->widget);
		parent->children_tail = &(new_widget->widget);
	}
	else
	{
		tmp1 = parent->children_head; 
		while (tmp1->next_sibling != NULL)
		{
			tmp1 = tmp1->next_sibling;
		}
		parent->children_tail = &(new_widget->widget);
		tmp1->next_sibling = &(new_widget->widget);
	}
	ei_widget_t *tmp = parent->next_sibling;
	while(tmp != NULL)
	{
		tmp = tmp->next_sibling;
	}
	tmp->next_sibling = new_widget;
	new_widget->widget.children_head = NULL;
	new_widget->widget.children_tail = NULL;
	}
	class->setdefaultsfunc;
	return (ei_widget_t*) new_widget;

}

void			ei_widget_destroy		(ei_widget_t*		widget)
{
	if (widget->children_head == NULL)
	{
		free(widget);
	}
	else
	{
		ei_widget_destroy (widget->children_head);
	}
}


ei_widget_t*		ei_widget_pick			(ei_point_t*		where)
{
}




void			ei_frame_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor)
{

	ei_frame_t* frame = (ei_frame_t*) widget;
	if (requested_size != NULL)
		frame->widget.requested_size = *requested_size;
	if (color != NULL)
		frame->color = *color;
	if (border_width != NULL)
		frame->border_size = *border_width;
	if (relief!=NULL)
		frame->relief = *relief;
	if (text!=NULL)
		frame->texte = *text;
	if (text_font!=NULL)
		frame->font = *text_font;
	if (text_color!=NULL)
		frame->text_color = text_color;
	if (text_anchor!=NULL)
		frame->text_anchor = *text_anchor;
	if (img != NULL)
		frame->img = *img;
	if (img_rect != NULL)
		frame->img_rect = *img_rect;
	if (img_anchor!=NULL)
	 	frame->img_anchor = *img_anchor;
}


void			ei_button_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 int*			corner_radius,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor,
							 ei_callback_t*		callback,
							 void**			user_param)
{
}


void			ei_toplevel_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size)
{
}
