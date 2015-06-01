#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "polygon.h"
#include "ei_types.h"


bool Comp_side (ei_side_t c1, ei_side_t c2)
{
	if (c1.ymax==c2.ymax && c1.x_ymin==c2.x_ymin && c1.rec_pente==c2.rec_pente)
		return true;
	else
		return false;
}

TCA *add_TCA (TCA *a, ei_side_t c)
{
	if (a->head != NULL)
	{
		cel_TCA* newcel = malloc(sizeof(cel_TCA));
		newcel->side = c;
		newcel->next = a->head;
		a->head = newcel;

	}
	else
	{
		a->head = malloc(sizeof(cel_TCA));
		a->head->side = c;
		a->head->next = NULL;
	}
	return a;
}
TCA *del_TCA (TCA *a, ei_side_t c)
{
	if (a->head == NULL)
		return NULL;

	TCA* tmp;
	if (Comp_side(a->head->side,c))	
	{
			tmp->head = a->head->next;
			free(a->head);
			tmp = del_TCA(tmp, c);
			return tmp;
	}
	else
	{
		a->head = a->head->next;
		a = del_TCA(a,c);
		return a;
	}
}
