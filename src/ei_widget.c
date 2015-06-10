#include "ei_draw.h"
#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "widgetbutton.h"
#include "widgetframe.h"
#include "widgettoplevel.h"
#include "ei_placer.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ei_application.h"
#include "global.h"


ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent)
{
	ei_widgetclass_t *class = ei_widgetclass_from_name(class_name);
	if (class == NULL)
	{
		return NULL;
		exit(0);
	}

	ei_widget_t *new_widget = class->allocfunc();

	new_widget->wclass = class;
	new_widget->parent = parent;
	new_widget->children_head = NULL;
	new_widget->children_tail = NULL;
	new_widget->next_sibling = NULL;
	new_widget->placer_params = NULL;
	if (parent != NULL)
	{
		//Le widget créé est le seul fils de parent :
		if (parent->children_head == NULL)
		{
			parent->children_head = new_widget;
			parent->children_tail = new_widget;
		}
		else
		{
			// ei_widget_t* tmp = parent->children_tail;
			// parent->children_tail->next_sibling = new_widget;
			// parent->children_tail = new_widget;

			(parent->children_tail->next_sibling) = new_widget;
			(parent->children_tail) = new_widget;

		}
	}
	/* if (strcmp(class_name,"toplevel") == 0 ) */
	/* { */
	/* 	ei_widget_t* title = ei_widget_create("frame",new_widget); */
	/* 	ei_widget_create("button",title); */
	/* 	ei_widget_create("frame",new_widget); */
	/* 	ei_widget_create("button",new_widget); */

	/* } */

	class->setdefaultsfunc(new_widget);
	new_widget->pick_id = widget_id_pick;
	new_widget->pick_color = malloc(sizeof(ei_color_t));
	*(new_widget->pick_color) = id_to_color(ei_app_root_surface(),widget_id_pick);
	widget_id_pick++;
	return new_widget;
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


/* static ei_widget_t* pick_rec(ei_widget_t* widget_pick, uint32_t *pixel_ptr) */
/* { */
/* 	if (widget_pick != NULL) */
/* 	{ */
/* 		if (widget_pick->pick_id == *pixel_ptr) */
/* 			return widget_pick; */
/* 		else { */
/* 			if (widget_pick->next_sibling != NULL) */
/* 				return pick_rec(widget_pick->next_sibling,pixel_ptr); */

/* 			if (widget_pick->children_head != NULL) */
/* 				return pick_rec(widget_pick->children_head,pixel_ptr); */
/* 		} */
/* 	} */
/* 	else */
/* 		return NULL; */
/* } */

/* ei_widget_t*		ei_widget_pick			(ei_point_t*		where) */
/* { */
/* 	ei_widget_t* widget; */
/* 	widget = ei_app_root_widget(); */
/* 	uint8_t* pixel_ptr = hw_surface_get_buffer(picking_surface)  */
/* 	+ (	where->y*hw_surface_get_size(picking_surface).width + where->x)*4; */
/* 	int ir,ig,ib,ia; */
/* 	hw_surface_get_channel_indices(picking_surface,&ir,&ig,&ib,&ia); */
/* 	uint8_t *pr,*pg,*pb; */
/* 	pr = pixel_ptr + ir; */
/* 	pg = pixel_ptr + ig; */
/* 	pb = pixel_ptr + ib; */

/* 	uint32_t color = (*pb) + 256*(*pg) + 256*256*(*pr); */
/* 	if (pick_rec(widget,&color)!=ei_app_root_widget())	 */
/* 	return pick_rec(widget,&color); */
/* 	else */
/* 	return NULL; */
/* } */


static ei_widget_t* pick_rec(ei_widget_t* widget_pick, ei_point_t* where)
{
	if (widget_pick->children_head == NULL)
		return widget_pick;
	/* Le click est dans la zone du widget */
	/* Determinons dans quel fils le click a eu lieu */
	ei_widget_t *widget_pointe = widget_pick;
	ei_widget_t *widget_cour = widget_pick->children_head;
	
	while(widget_cour != NULL){
		//Si un voisin plus avancé contient le click, c'est lui ou un de ses fils.
		if (( widget_cour->screen_location.top_left.x <= where->x) && 
		    ( widget_cour->screen_location.top_left.y <= where->y) &&
		    ((widget_cour->screen_location.top_left.x + widget_cour->screen_location.size.width) >= where->x) &&
		    ((widget_cour->screen_location.top_left.y + widget_cour->screen_location.size.height) >= where->y))
		{
			widget_pointe = widget_cour;
		}
		widget_cour = widget_pick->next_sibling;
	}
        // Si rien parmi les fils, on doit retourner widget_pick
	if (widget_pointe == widget_pick)
		return widget_pick;

	widget_pointe = pick_rec(widget_pointe, where);
	return widget_pointe;
			
}

ei_widget_t*		ei_widget_pick			(ei_point_t*		where)
{
	ei_widget_t *widget = ei_app_root_widget();

	if (pick_rec(widget, where)!=ei_app_root_widget())
		return pick_rec(widget, where);
	else
		return NULL;
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
	else 
		frame->relief = 0;
	if (text!=NULL)
		frame->texte = *text;
	if (text_font!=NULL)
		frame->font = *text_font;
	if (text_color!=NULL)
		frame->text_color = text_color;
	if (text_anchor!=NULL)
		frame->text_anchor = *text_anchor;
	if (img != NULL)
		frame->img = img;
	if (img_rect != NULL)
		frame->img_rect = *img_rect;
	if (img_anchor!=NULL)
	 	frame->img_anchor = *img_anchor;
}


void	ei_button_configure(     ei_widget_t*		widget,
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
	ei_button_t* button = (ei_button_t*) widget;
	if (requested_size != NULL)
		button->widget.requested_size = *requested_size;
	if (color != NULL)
		button->color = *color;
	if (border_width != NULL)
		button->border_size = *border_width;
	if (relief!=NULL)
		button->relief = *relief;
	else 
		button->relief = 0;
	if (text!=NULL)
		button->texte = *text;
	if (text_font!=NULL)
		button->font = *text_font;
	if (text_color!=NULL)
		button->text_color = text_color;
	if (text_anchor!=NULL)
		button->text_anchor = *text_anchor;
	if (img != NULL)
		button->img = *img;
	if (img_rect != NULL)
		button->img_rect = *img_rect;
	if (img_anchor!=NULL)
	 	button->img_anchor = *img_anchor;
	if (corner_radius!=NULL)
		button->corner_radius = corner_radius;
	if (callback!=NULL)
		button->callback = *callback;
	if (user_param != NULL)
		button->user_param = *user_param;
}

void close_window(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
	
}

void	ei_toplevel_configure	(ei_widget_t*		widget,
				 ei_size_t*		requested_size,
				 ei_color_t*		color,
				 int*			border_width,
				 char**			title,
				 ei_bool_t*		closable,
				 ei_axis_set_t*		resizable,
				 ei_size_t**		min_size)
{
	ei_toplevel_t* toplevel = (ei_toplevel_t*) widget;
	/* ------------------------------------------------------- */
	/* Les barres d'en tete font 25px de hauteur  */
	/* Assignations */
	if (requested_size != NULL)
		toplevel->widget.requested_size = *requested_size;
	if (color != NULL)
		toplevel->color = *color;
	if (border_width != NULL)
		toplevel->border_size = *border_width;
	if (title != NULL)
		toplevel->title = *title;
	if (closable != NULL)
		toplevel->closable = *closable;
	if (resizable != NULL)
		toplevel->resizable = *resizable;
	if (min_size != NULL)
		toplevel->min_size = *min_size;

	/* Bouton de fermeture */
	if (*closable == EI_TRUE){
		ei_widget_t    *closing_button;
		ei_anchor_t	button_anchor           = ei_anc_northwest;
		int		button_x		= 3;
		int		button_y		= 2;
		float		button_rel_x		= 0.0;
		float		button_rel_y		= 0.0;
		ei_color_t	button_color		= {0, 0, 0, 0xff};
		button_color.red = 0xff;
		ei_relief_t	button_relief		= ei_relief_raised;
		int		button_border_width	= 3;
		ei_callback_t	button_callback 	= close_window;
		ei_size_t       button_size             = {20, 20};
		int             button_radius           = 10;
	
		closing_button = ei_widget_create("button", widget);
		ei_button_configure(closing_button, 
				    &button_size, 
				    &button_color,
				    &button_border_width,
				    &button_radius,
				    &button_relief,
				    NULL, NULL,  NULL,  NULL, NULL, NULL, NULL,
				    &button_callback, NULL);
		ei_place(closing_button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, NULL, NULL);
	}
	/* /\* -------------------------------------------------------- *\/ */
	/* ei_size_t title_size; */
	/* ei_color_t title_color = {0xAA,0xAA,0xAA,255}; */
	/* title_size.height = 30; */
	/* ei_font_t title_font = ei_default_font; */
	/* ei_color_t black = {0xff,0xff,0xff,255}; */
	/* ei_widget_t* titlebar = toplevel->widget.children_head; */
	/* if (requested_size != NULL) */
	/* { */
	/* 	toplevel->widget.requested_size = *requested_size; */
	/* 	title_size.width = requested_size->width; */
	/* } */
	/* if (color != NULL) */
	/* 	toplevel->color = *color; */
	/* if (border_width != NULL) */
	/* 	toplevel->border_size = *border_width; */
	/* if (title != NULL) */
	/* { */
	/* 	toplevel-> title = *title; */
	/* 	//titlebar */
	/* ei_frame_configure(titlebar,&title_size,&title_color, */
	/* 	NULL,NULL,&toplevel->title,&title_font,&black,NULL,NULL,NULL,NULL); */
	/* ei_place(titlebar,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL); */
	/* } */
	/* else */
	/* { */
	/* ei_frame_configure(titlebar,&title_size,&title_color, */
	/* 	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);	 */
	/* } */
	/* if (closable !=NULL) */
	/* 	toplevel->closable = *closable; */
	/* if (toplevel->closable == EI_TRUE) */
	/* { */
	/* 	//close button */
	/* 	int close_corner_radius = 10; */
	/* 	ei_size_t close_size; */
	/* 	close_size.width = 20; */
	/* 	close_size.height = 20; */
	/* 	ei_color_t close_color = {0xff,0,0,0}; */
	/* 	int close_border = 5; */
	/* 	ei_relief_t close_relief = ei_relief_raised; */
	/* 	ei_button_configure(titlebar->children_head,&close_size,&close_color, */
	/* 		&close_border,&close_corner_radius,&close_relief,NULL,NULL,NULL, */
	/* 		NULL,NULL,NULL,NULL,NULL,NULL); */
	/* 	int close_x = 2*toplevel->border_size; */
	/* 	int close_y = 2*toplevel->border_size; */
	/* 	ei_place(titlebar->children_head,NULL,&close_x,&close_y, */
	/* 		&close_size.width,&close_size.height,NULL,NULL,NULL,NULL);	 */
	/* } */
	/* if (resizable != NULL) */
	/* 	toplevel->resizable = *resizable; */
	/* if (min_size != NULL) */
	/* 	toplevel->min_size = *min_size; */

	/* //inside frame */
	/* ei_color_t in_frame_color = toplevel->color; */
	/* ei_frame_configure(toplevel->widget.children_head->next_sibling, */
	/* 	&toplevel->requested_size,&in_frame_color,&toplevel->border_size,NULL, */
	/* 	NULL,NULL,NULL,NULL,NULL,NULL,NULL); */
	/* int y = title_size.height; */
	/* ei_place(toplevel->widget.children_head->next_sibling,NULL,NULL,&y, */
	/* 	&toplevel->widget.screen_location.size.width, */
	/* 	&toplevel->widget.screen_location.size.height,NULL,NULL,NULL,NULL); */
	
}
