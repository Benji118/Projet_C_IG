#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"
#include <stdint.h>
#include <math.h>

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
{
	int *ir, *ig, *ib, *ia;
	ir=malloc(sizeof(*ir));
	ig=malloc(sizeof(*ig));
	ib=malloc(sizeof(*ib));
	ia=malloc(sizeof(*ia));
	assert(ir != NULL);
	assert(ig != NULL);
	assert(ib != NULL);
	assert(ia != NULL);
	hw_surface_get_channel_indices(surface,ir,ig,ib,ia);
	uint32_t rouge=(uint32_t) color->red;
	uint32_t vert=(uint32_t) color->green;
	uint32_t bleu=(uint32_t) color->blue;
	uint32_t alpha=(uint32_t) color->alpha;
	rouge=rouge<<(*ir)*8;
	vert=vert<<(*ig)*8;
	bleu=bleu<<(*ib)*8;
	if (hw_surface_has_alpha(surface)==EI_TRUE) {
		alpha=alpha<<(*ia)*8;
	} else {
		alpha=0;
	}
	/* Libération des pointeurs */
	free(ig); free(ib); free(ir); free(ia);

	return rouge+vert+bleu+alpha;
}

void			ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) 
{
	ei_linked_point_t *sent=malloc(sizeof(ei_linked_point_t));
	assert(sent!=NULL);
	sent=first_point;
	/* Variables pour l'algo */
	uint32_t x1,x2,y1,y2;
	uint32_t dx,dy,e;

	/* Taille surface */
	/* A utiliser pour vérifier et parcourir le tableau */ 
	ei_size_t taille=hw_surface_get_size(surface);

	/* Recuperer l'adresse du pixel (0,0) */
	uint32_t* pixel_ptr=(uint32_t*)hw_surface_get_buffer(surface);
	uint32_t* a;
	
	/* Première boucle de parcours des points */
	while (sent->next!=NULL) {
		/* Initialisation des varibles pour chaque segment */
		x1=(uint32_t)sent->point.x;
		x2=(uint32_t)sent->next->point.x;
		y1=(uint32_t)sent->point.y;
		y2=(uint32_t)sent->next->point.y;
		dx=x2-x1;
		dy=y2-y1;
		e=dx;
		dx=2*e;
		dy=2*dy;

		/* Boucle de traitement, segment par segment */
		/* Traitement */
		while (x1!=x2) {
			/* Trouver comment acceder au tableau */
			a=pixel_ptr;
			a=a+y1*taille.width+x1;
			*a=ei_map_rgba(surface,&color);
			x1=x1+1;
			e=e+dy;
			if (2*e>dx) {
				y1=y1+1;
				e=e-dx;
			}
		}
		/* Dessiner dans le dernier pixel qui n'est pas pris en compte dans la boucle */

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
		//hw_surface_lock(surface);

		int taille_TC = hw_surface_get_size(surface).height;
		ei_side_t* TC= calloc(taille_TC,sizeof(ei_side_t));

		//Initialisation de la scanline
		int y_cour = 0;
		ei_linked_point_t* tmp = first_point;
		while ((tmp->point).y != y_cour)
		{
			y_cour++;
		}
		tmp = first_point;
		while (tmp != NULL)
		{
			//Cas cote horizontal
			if ((tmp->point).y==(tmp->next->point).y)
				y_cour++;

			//Remplissage de TC
			if ((tmp->point).y==y_cour || (tmp->next->point).y)
			{
				if ((tmp->point).y > (tmp->next->point).y)
				{

					TC[y_cour].ymax = (tmp->point).y;
					TC[y_cour].x_ymin = (tmp->next->point).x;
					TC[y_cour].rec_pente = ((tmp->next->point).x - (tmp->point).x) / 
											 ((tmp->next->point).y - (tmp->point).y);
				}
				else
				{
					TC[y_cour].ymax = (tmp->next->point).y;
					TC[y_cour].x_ymin = (tmp->point).x;
					TC[y_cour].rec_pente = ((tmp->next->point).x - (tmp->point).x) / 
											 ((tmp->next->point).y - (tmp->point).y);				
		
				}
				tmp = tmp->next;
				y_cour++;
			}
		else{
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
