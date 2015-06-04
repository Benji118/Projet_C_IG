#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "polygon.h"
#include "ei_types.h"


ei_bool_t comp_side (ei_side_t c1, int y) 
{
	if (c1.ymax==y)/* && c1.x_ymin==c2.x_ymin && c1.rec_pente==c2.rec_pente*/ {
		return EI_TRUE;
	} else {
		return EI_FALSE;
	}
}

TCA *create_TCA()
{
	TCA *new_TCA=malloc(sizeof(TCA));
	assert(new_TCA!=NULL);
	new_TCA->head=NULL;
	return new_TCA;
}

void add_TCA (TCA *a, ei_side_t c)
{
	if (a->head!=NULL) {
		cell_TCA *new_cell=malloc(sizeof(cell_TCA));
		new_cell->side=c;
		new_cell->x_inter=c.x_ymin;
		new_cell->next=a->head;
		a->head=new_cell;
	} else {
		cell_TCA *new_cell=malloc(sizeof(cell_TCA));
		new_cell->side=c;
		new_cell->x_inter=c.x_ymin;
		new_cell->next=NULL;
		a->head=new_cell;
	}
}

void del_TCA (TCA *a, int y)
{
	cell_TCA *cour=a->head;
	cell_TCA *prec=a->head;
	while (cour!=NULL) {
		if (cour->side.ymax==y) {
			if (prec==cour) {
				a->head=a->head->next;
				cour=a->head;
				prec=a->head;
			} else {
				prec->next=cour->next;
				cour=cour->next;
			}
		} else {
			if (prec!=cour) {
				prec=prec->next;
			}
			cour=cour->next;
		}
	}
}

TCA *sort_TCA (TCA* a)
{
	// Création d'une nouvelle TCA
	TCA *tri=create_TCA();
	cell_TCA *sent_TCA=a->head;
	while (sent_TCA!=NULL) {
		cell_TCA *new=malloc(sizeof(cell_TCA));
		assert(new!=NULL);
		*new=*sent_TCA;
		//printf("sent_TCA : %d\n",sent_TCA->x_inter);
		if (tri->head!=NULL) {
			//	printf("tri head : %d\n",tri->head->x_inter);
		}
		if (tri->head==NULL) {			
			new->next=NULL;
			tri->head=new;
		} else if ((tri->head->x_inter)>=(sent_TCA->x_inter)) {
			//printf("tete\n");
			// Insertion en tete
			new->next=tri->head;
			tri->head=new;
		} else {
			cell_TCA *sent_tri=tri->head;
			ei_bool_t find=EI_FALSE;
			while (find!=EI_TRUE) {
				if (sent_tri->next==NULL) {
					// Insertion en queue
					new->next=NULL;
					sent_tri->next=new;
					find=EI_TRUE;
				} else if (sent_tri->next->x_inter>=sent_TCA->x_inter) {
					find=EI_TRUE;
					new->next=sent_tri->next;
					sent_tri->next=new;
				} else {
					sent_tri=sent_tri->next;
				}
			}
		}
		sent_TCA=sent_TCA->next;
	}
	return tri;
}

void draw_pixel(ei_surface_t surface,
		uint32_t x,
		uint32_t y,
		const ei_color_t color,
		uint32_t* pixel_ptr_origin,
		const ei_rect_t* clipper)
{
	// Clipper "brut" 4 tests
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
