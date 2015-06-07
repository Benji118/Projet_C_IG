#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "widgetframe.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "global.h"


void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{
	if (list_class == NULL)
	{
		list_class = widgetclass;
	}
	else
	{
		ei_widgetclass_t* tmp = list_class;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = widgetclass;
		free(tmp);
	}
}


ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name)
{
	if (list_class != NULL)
	{
	ei_widgetclass_t* res = list_class;
	while (res!=NULL)
	{
		if (strcmp(res->name,name)==0)
		{
			return res;
		}
		res = res->next;
	}
	return NULL;
	}
	else
	return NULL;
}


void			ei_frame_register_class 	()
{	
	ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
	strcpy(frame->name,"frame");
	frame->allocfunc = ei_frame_allocfunc;
	frame->releasefunc = ei_frame_releasefunc;
	frame->drawfunc = ei_frame_drawfunc;
	frame->setdefaultsfunc = ei_frame_setdefaultsfunc;
	frame->geomnotifyfunc = ei_frame_geomnotifyfunc;
	//frame->handlefunc = ei_frame_handlefunc;

	ei_widgetclass_register(frame);
}


void			ei_button_register_class 	()
{
}


void			ei_toplevel_register_class 	()
{
}
