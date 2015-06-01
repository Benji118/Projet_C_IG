#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"
//#include "polygon.h"
#include <stdint.h>

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
	uint32_t rouge_final, vert_final, bleu_final;
	rouge_final=rouge<<(*ir)*8;
	vert_final=vert<<(*ig)*8;
	bleu_final=bleu<<(*ib)*8;
	/* Gestion de la transparence */
	if (hw_surface_has_alpha(surface)==EI_TRUE) {
		alpha=alpha<<(*ia)*8;
	} else {
		alpha=0;
	}
	/* Libération des pointeurs */
	free(ig); free(ib); free(ir); free(ia);

	return rouge_final+vert_final+bleu_final+alpha;
}

void draw_pixel(ei_surface_t surface,
                                int x,
                                int y,
                                ei_color_t color)
{
        uint32_t *pixel = (uint32_t*)(hw_surface_get_buffer(surface)
                                        + y * hw_surface_get_size(surface).width * sizeof(uint32_t)
                                        + x * sizeof(uint32_t));
        *pixel =        ei_map_rgba(surface,&color);

}

void                    ei_draw_polyline        (ei_surface_t                   surface,
                                                 const ei_linked_point_t*       first_point,
                                                 const ei_color_t               color,
                                                 const ei_rect_t*               clipper)
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
                // sent->next != NULL
                /* Dessiner dans le dernier pixel qui n'est pas pris en compte dans la boucle */

                sent=sent->next;
        }
}


void			ei_draw_polygon		(ei_surface_t			surface,
	const ei_linked_point_t*	first_point,
	const ei_color_t		color,
	const ei_rect_t*		clipper)
{	
	if (first_point!=NULL && first_point->next!=NULL)
	{
		ei_side_t*** TC = calloc(hw_surface_get_size(surface).height,sizeof(ei_side_t*));
		TC[(first_point->point).y][0] = malloc(sizeof(ei_side_t));
		if ((first_point->point).y!=(first_point->next->point).y)
		{
			if ((first_point->point).y > (first_point->next->point).y)
			{
				TC[(first_point->point).y][0]->ymax = (first_point->point).y;
				TC[(first_point->point).y][0]->x_ymin = (first_point->next->point).x;
			}
			else if ((first_point->point).y < (first_point->next->point).y)
			{
				TC[(first_point->point).y][0]->ymax = (first_point->next->point).y;
				TC[(first_point->point).y][0]->x_ymin = (first_point->point).x;
			}
			TC[(first_point->point).y][0]->rec_pente = ((first_point->next->point).x - (first_point->point).x) /
												 ((first_point->next->point).y - (first_point->point).y);
		}
		int inc =0;
		ei_linked_point_t* tmp = first_point->next;
		while (tmp != NULL)
		{
			if (TC[(tmp->point).y][inc]==NULL)
			{
				TC[(tmp->point).y][inc] = malloc(sizeof(ei_side_t));
				if ((tmp->point).y!=(tmp->next->point).y)
				{
					if ((tmp->point).y > (tmp->next->point).y)
					{
						TC[(tmp->point).y][inc]->ymax = (tmp->point).y;
						TC[(tmp->point).y][inc]->x_ymin = (tmp->next->point).x;
					}
					else if ((tmp->point).y < (tmp->next->point).y)
					{
						TC[(tmp->point).y][inc]->ymax = (tmp->next->point).y;
						TC[(tmp->point).y][inc]->x_ymin = (tmp->point).x;
					}
					TC[(tmp->point).y][inc]->rec_pente = ((tmp->next->point).x - (tmp->point).x) /
					((tmp->next->point).y - (tmp->point).y);
				}
				inc = 0;
				tmp = tmp->next;
			}
			else
			{
				inc++;
			}
		}
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
	ei_size_t size=hw_surface_get_size(surface);
	uint32_t* pixel_ptr=(uint32_t*)hw_surface_get_buffer(surface);

	for (int i=0; i<(size.width*size.height) ;i++) {
		*pixel_ptr++=ei_map_rgba(surface,color);
	}
}

int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
}
