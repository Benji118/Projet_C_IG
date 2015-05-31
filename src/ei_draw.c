#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <hw_interface.h>

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
{
	int *ir, *ig, *ib, *ia;
	ir=malloc(sizeof(*ir));
	ig=malloc(sizeof(*ig));
	ib=malloc(sizeof(*ib));
	ia=malloc(sizeof(*ia));
	assert(ir);
	assert(ig);
	assert(ib);
	assert(ia);
	hw_surface_get_channel_indices(surface,ir,ig,ib,ia);
	printf("ir : %i\nig : %i\nib : %i\nia : %i\n",*ir, *ig, *ib, *ia);
	uint32_t rgb = 1;
	return rgb;
}

void			ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) 
{	
	ei_linked_point_t *sent=malloc(sizeof(ei_linked_point_t));
	assert(sent!=NULL);
	*sent=first_point;
	ei_point_t p1, p2;

	/* Variables pour l'algo */
	int x1,x2,y1,y2;
	int dx,dy,e;

	/* Taille surface */
	/* A utiliser pour vérifier */ 
	ei_size_t taille=hw_surface_get_size(surface);

	while (sent->next!=NULL) {		
		p1=sent->point;
		p2=sent->next->point;		
		x1=p1.x;
		x2=p2.x;
		y1=p1.y;
		y2=p2.y;
		dx=x2-x1;
		dy=y2-y1;
		e=dx;
		dx=2*e;
		dy=2*dy;

		/* Traitement */
		while (x1!=x2) {
			surface(x1,y1)=color;
			x1=x1+1;
			e=e+dy;
			if (2*e>dx) {
				y1=y1+1;
				e=e-dx;
			}
		}
		/* sent->next != NULL */
		sent=sent->next;		
	}
}

void			ei_draw_polygon		(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper)
{	
	if (first_point != NULL && first_point->next!=NULL)
	{
		hw_surface_lock(surface);

		int taille_TC = hw_surface_get_size(surface).height;
		ei_side_t* TC= calloc(taille_TC,sizeof(ei_side_t));

		//Initialisation de la scanline
		int y_cour = 0;
		ei_linked_point_t* tmp = first_point;
		while (tmp->(point).y != y_cour)
		{
			y_cour++;
		}
		tmp = first_point;
		while (tmp != NULL)
		{
			//Cas cote horizontal
			if (tmp->(point).y==tmp->next->(point).y)
				y_cour++;

			//Remplissage de TC
			if (tmp->(point).y==y_cour || tmp->next->(point).y)
			{
				if (tmp->(point).y > tmp->next->(point).y)
				{

					(TC[y_cour]).ymax = tmp->(point).y;
					(TC[y_cour]).x_ymin = tmp->next->(point).x;
					(TC[y_cour]).rec_pente = (tmp->next->(point).x-tmp->(point).x) / 
											 (tmp->next->(point).y-tmp->(point).y);
				}
				else
				{
					(TC[y_cour]).ymax = tmp->next->(point).y;
					(TC[y_cour]).x_ymin = tmp->(point).x;
					(TC[y_cour]).rec_pente = (tmp->next->(point).x-tmp->(point).x) / 
											 (tmp->next->(point).y-tmp->(point).y);				
		
				}
				tmp = tmp->next;
				y_cour++;
			}
		else{
				TC[y_cour]=NULL;
				y_cour++;
			}
		}
		free(tmp);
		//Initialisation de TCA
		ei_side_t* TCA = malloc(sizeof(ei_side_t));
		TCA = NULL;
		//Debut de l'algo
	}
}

void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
}

void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
}

int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
}
