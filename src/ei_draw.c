#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"
#include "polygon.h"
#include <stdint.h>

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
{
	int *ir, *ig, *ib, *ia;
	ir=malloc(sizeof(uint8_t));
	ig=malloc(sizeof(uint8_t));
	ib=malloc(sizeof(uint8_t));
	ia=malloc(sizeof(uint8_t));
	hw_surface_get_channel_indices(surface,ir,ig,ib,ia);
/*	printf("ir : %i\nig : %i\nib : %i\nia : %i\n",*ir, *ig, *ib, *ia);
*/	uint32_t rgb = 0 ;
	rgb |= color->red << *ir * sizeof(uint8_t);
	rgb |= color->green << *ig * sizeof(uint8_t);
	rgb |= color->blue << *ib * sizeof(uint8_t);
	rgb |= color->alpha << *ia * sizeof(uint8_t);
	return rgb;
}

void draw_pixel(ei_surface_t surface,
				int x,
				int y,
				ei_color_t color)
{
	uint32_t *pixel = (uint32_t*)(hw_surface_get_buffer(surface)
					+ y * hw_surface_get_size(surface).width * sizeof(uint32_t)
					+ x * sizeof(uint32_t));
	*pixel = 	ei_map_rgba(surface,&color);

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
		 sent->next != NULL 
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
}

int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
}
