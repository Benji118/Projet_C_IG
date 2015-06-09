#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "widgetframe.h"
#include "widgetbutton.h"
#include "widgettoplevel.h"
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
		widgetclass->next = malloc(sizeof(ei_widgetclass_t));
		widgetclass->next = list_class;
		list_class = widgetclass;
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
	}
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
	ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
	strcpy(button->name,"button");
	button->allocfunc = ei_button_allocfunc;
	button->releasefunc = ei_button_releasefunc;
	button->drawfunc = ei_button_drawfunc;
	button->setdefaultsfunc = ei_button_setdefaultsfunc;
	button->geomnotifyfunc = ei_button_geomnotifyfunc;
	//button->handlefunc = ei_button_handlefunc;

	ei_widgetclass_register(button);
}


void			ei_toplevel_register_class 	()
{
	ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));
	strcpy(toplevel->name,"toplevel");
	toplevel->allocfunc = ei_toplevel_allocfunc;
	toplevel->releasefunc = ei_toplevel_releasefunc;
	toplevel->drawfunc = ei_toplevel_drawfunc;
	toplevel->setdefaultsfunc = ei_toplevel_setdefaultsfunc;
	toplevel->geomnotifyfunc = ei_toplevel_geomnotifyfunc;
	//topelevel->handlefunc = ei_topelevel_handlefunc;

	ei_widgetclass_register(toplevel);
}
