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
		newcel->x_inter = c.x_ymin;
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

void swap_TCA (cel_TCA *cel1, cel_TCA *cel2)
{
	cel_TCA *tmp = cel1->next;
	cel1->next =cel2->next;
	cel2->next = tmp;
}

TCA *sort_TCA (TCA* a)
{
	if (a !=NULL)
	{
		cel_TCA *tmp3;
		int min;
		for (cel_TCA *tmp=a->head;tmp!=NULL;tmp=tmp->next)
		{
			tmp3=tmp;
			min=tmp->x_inter;
			for (cel_TCA *tmp1=tmp->next; tmp1!=NULL;tmp1=tmp1->next)
			{
				if(min >tmp1->x_inter)
				{
					tmp3=tmp1;
					min=tmp3->x_inter;
				}
			}
			swap_TCA(tmp3,tmp);
		}
		return a;
	}
	else
	{
		return NULL;
	}
}