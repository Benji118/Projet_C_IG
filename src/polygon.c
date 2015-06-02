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

void draw_pixel(ei_surface_t surface,
		uint32_t x,
		uint32_t y,
		const ei_color_t color,
		uint32_t* pixel_ptr_origin,
		const ei_rect_t* clipper)
{
	/* Clipper "brut" 
	   4 tests */
	if (clipper!=NULL) 
	{
		if ((x>clipper->top_left.x) &
		    (y>clipper->top_left.y) &
		    (x<(clipper->top_left.x+clipper->size.width)) &
		    (y<(clipper->top_left.y+clipper->size.height)))
		{    
			ei_size_t taille=hw_surface_get_size(surface);
			uint32_t* pixel_ptr=pixel_ptr_origin+y*taille.width+x;
			*pixel_ptr=ei_map_rgba(surface,&color);
		}
	} else {
		ei_size_t taille=hw_surface_get_size(surface);
		uint32_t* pixel_ptr=pixel_ptr_origin+y*taille.width+x;
		*pixel_ptr=ei_map_rgba(surface,&color);
	}
		
}

uint32_t alpha_effect(ei_surface_t surface_source,
					  ei_surface_t surface_dest,
	 				  uint32_t *pixel_source,
	 				  uint32_t *pixel_dest)
{
	int irs,igs,ibs,ias, ird,igd,ibd,iad;
	hw_surface_get_channel_indices(surface_source,&irs,&igs,&ibs,&ias);
	hw_surface_get_channel_indices(surface_dest,&ird,&igd,&ibd,&iad);

	uint32_t Sr,Sg,Sb,Sa, Pr,Pg,Pb,Pa;
	Pr = *pixel_source << irs*8;
	Pg = *pixel_source << igs*8;
	Pb = *pixel_source << ibs*8;
	Pa = *pixel_source << ias << 8*ias;

	Sr = *pixel_dest << ird*8;
	Sg = *pixel_dest << igd*8;
	Sb = *pixel_dest << ibd*8;
	Sa = 0xF << iad*8;

	uint32_t Sr_final,Sg_final,Sb_final,Sa_final;
	Sr_final = (Pa*Pr + (255-Pa)*Sr)/255;
	Sg_final = (Pa*Pg + (255-Pa)*Sg)/255;
	Sb_final = (Pa*Pb + (255-Pa)*Sb)/255;
	Sa_final = Sa;

	return Sr_final+Sg_final+Sb_final+Sa_final;



}
