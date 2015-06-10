#include "ei_draw.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_types.h"
#include "polygon.h"
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

void                    ei_draw_polyline        (ei_surface_t                   surface,
                                                 const ei_linked_point_t*       first_point,
                                                 const ei_color_t               color,
                                                 const ei_rect_t*               clipper)
{
	ei_linked_point_t *sent=malloc(sizeof(ei_linked_point_t));
	assert(sent!=NULL);
	*sent=*first_point;
	/* Variables pour l'algo */
	uint32_t x1,x2,y1,y2;
	int32_t dx,dy,e;

	/* Recuperer l'adresse du pixel (0,0) */
	uint32_t* pixel_ptr=(uint32_t*)hw_surface_get_buffer(surface);

	/* Première boucle de parcours des points */
	while (sent->next != NULL) {
		/* Initialisation des varibles pour chaque segment */
		x1=(uint32_t)sent->point.x;
		x2=(uint32_t)sent->next->point.x;
		y1=(uint32_t)sent->point.y;
		y2=(uint32_t)sent->next->point.y;
		dx=x2-x1;
		dy=y2-y1;
		if (dx!=0) {
			if (dx>0) {
				if (dy!=0) {
					if (dy>0) {
						//Vecteur oblique 1er cadran
						if (dx>=dy) {
							//1er octant
							e=dx;
							dx=2*e;
							dy=2*dy;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1+1;
								e=e+dy;
								if (2*e>dx) {
									y1=y1+1;
									e=e-dx;
								}
							}
						} else {
							//2eme octant
							e=dy;
							dx=2*dx;
							dy=2*e;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (y1!=y2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								y1=y1+1;
								e=e-dx;
								if (e<0) {
									x1=x1+1;
									e=e+dy;
								}
							}
						}
					} else {
						//Vecteur oblique 4eme cadran
						if (dx+dy>=0) {
							//8e octant
							e=dx;
							dx=2*e;
							dy=2*dy;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1+1;
								e=e+dy;
								if (e<0) {
									y1=y1-1;
									e=e+dx;
								}
							}
						} else {
							//7e octant
							e=dy;
							dx=2*dx;
							dy=2*e;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (y1!=y2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								y1=y1-1;
								e=e+dx;
								if (e>0) {
									x1=x1+1;
									e=e+dy;
								}
							}
						}
					}
				} else {
					//Vecteur horizontal vers la droite
					while (x1!=x2) {
						draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
						x1=x1+1;
					}				
				}
			} else {				
				if (dy!=0) {
					if (dy>0) {
						//2e cadran
						if (dx+dy<=0) {
							//4e octant
							e=dx;
							dx=2*e;
							dy=2*dy;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1-1;
								e=e+dy;
								if (e>=0) {
									y1=y1+1;
									e=e+dx;
								}
							}
						} else {
							//3e octant
							e = dy;
							dx = 2*dx;
							dy = 2*e;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (y1!=y2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								y1=y1+1;
								e=e+dx;
								if (e<=0) {
									x1=x1-1;
									e=e+dy;
								}
							}	
						}
					} else {
						//3e cadran
						if (dx<=dy){
							//5e octant
							e=dx;
							dx=2*e;
							dy=2*dy;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1-1;
								e=e-dy;
								if (e>=0) {
									y1=y1-1;
									e=e+dx;
								}
							}
						} else {
							//6e octant
							e=dy;
							dy=2*e;
							dx=2*dx;
							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (y1!=y2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								y1=y1-1;
								e=e-dx;
								if (e>=0) {
									x1=x1-1;
									e=e+dy;
								}
							}
						}
					} 
				} else {
					//Vecteur horizontal vers la gauche
					while (x1!=x2) {
						draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
						x1=x1-1;
					}
				}
			}
		} else {
			if (dy>0) {
				//Vecteur vertical croissant
				while (y1!=y2) { 
					draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
					y1=y1+1;
				}
			} else {
				//Vecteur vertical decroissant
				while (y1!=y2) {
					draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
					y1=y1-1;
				}
			}
		}
		/* Dessiner dans le dernier pixel qui n'est pas pris en compte dans la boucle */
		draw_pixel(surface,x2,y2,color,pixel_ptr,clipper);
		
		sent=sent->next;       
	}
}

void			ei_draw_polygon		(ei_surface_t			surface,
	const ei_linked_point_t*	first_point,
	const ei_color_t		color,
	const ei_rect_t*		clipper)
{	
	// Problème du polygone nul
        if (first_point!=NULL) {
		// Création d'un tableau de pointeurs vers des cotés (TC)
		ei_size_t size=hw_surface_get_size(surface);
		ei_side_t* TC[size.height];	
		for (int i=0; i<size.height; i++) {
			TC[i]=NULL;
		}
		ei_linked_point_t *sent;
		sent=malloc(sizeof(ei_linked_point_t));
		assert(sent!=NULL);
		*sent=*first_point;
		ei_point_t p1,p2;
		int y_max;
		float x_ymin;
		// num_scan représente la première scanline d'intersection 
		int num_scan;
		while (sent->next!=NULL) {
			p1=sent->point;
			p2=sent->next->point;
			// Si le segment est horizontal on ne l'ajoute pas dans TC 
			if (p1.y!=p2.y) {
				// On cherche y_max 
				if (p1.y<p2.y) {
					y_max=p2.y;
					x_ymin=(float)p1.x;
					num_scan=p1.y;
				} else {
					y_max=p1.y;
					x_ymin=(float)p2.x;
					num_scan=p2.y;
				}
				// Création du nouveau coté 
				ei_side_t *new_side=malloc(sizeof(ei_side_t));
				assert(new_side!=NULL);
				new_side->ymax=y_max;
				new_side->x_ymin=x_ymin;
				new_side->inv_pente=((float)p2.x-(float)p1.x)/((float)p2.y-(float)p1.y);
				new_side->next=NULL;
				// On remplit TC avec le nouveau côté
				if (TC[num_scan]!=NULL) {
					ei_side_t *sent_tab=TC[num_scan];
					while (sent_tab->next!=NULL) {
						sent_tab=sent_tab->next;               
					}					
					sent_tab->next=new_side;
				} else {
					TC[num_scan]=new_side;
				}
			}		
			// On change de coté
			sent=sent->next;
		}

		// Recuperer l'adresse du pixel (0,0)
		uint32_t* pixel_ptr=(uint32_t*)hw_surface_get_buffer(surface);

		//Creation de la TCA
		TCA *TCA=create_TCA();

		// Boucle des scanlines
		for (int k=0; k<size.height; k++) {
			ei_side_t *sent_tab=TC[k];
			while (sent_tab!=NULL) {
				// A FAIRE : Supression dans TC
				// A FAIRE : Gérer la libération 
				// Ajout en tete de TCA 
				add_TCA(TCA,*sent_tab);	      
				sent_tab=sent_tab->next;
			}
			free_TC(TC[k]);
			// On supprime les éléments tels que y_max=k
			del_TCA(TCA,k);
			// On trie la TCA 
			TCA=sort_TCA(TCA);
			
			// Remplissage de la scanline
			cell_TCA *senti_TCA=TCA->head;
			while (senti_TCA!=NULL && senti_TCA->next != NULL) {
				float inter1=ceilf(senti_TCA->x_inter);
				float inter2=floorf(senti_TCA->next->x_inter);
				// Gestion des règles de remplissage
				int inf=(int) inter1;
				int sup=(int) inter2;
				// Dessin entre deux intersections successives intersections
				for (int m=inf; m<=sup; m++) {
					draw_pixel(surface,m,k,color,pixel_ptr,clipper);
				}
				senti_TCA=senti_TCA->next->next;
			}
			cell_TCA *maj_x_inter=TCA->head;
			while (maj_x_inter!=NULL) {
				// Mise a jour des x_inter
				maj_x_inter->x_inter=maj_x_inter->x_inter+maj_x_inter->side.inv_pente;
				// On passe au coté suivant dans TCA
				maj_x_inter=maj_x_inter->next;			       		  
			}
		}
	}
}

static ei_rect_t* inter_rect(ei_rect_t rect1, ei_rect_t rect2)
{
	// On cherche le top left de l'intersection
	ei_point_t top_inter;
	ei_size_t size_inter;
	// On prend le max des top_left.x
	if (rect1.top_left.x<rect2.top_left.x) {
		top_inter.x=rect2.top_left.x;
		if (top_inter.x+rect2.size.width<rect1.top_left.x+rect1.size.width) {
			size_inter.width=rect2.size.width;
		} else {
			size_inter.width=rect1.top_left.x+rect1.size.width-top_inter.x;
		}
	} else if (rect1.top_left.x>rect2.top_left.x) {
		top_inter.x=rect1.top_left.x;
	        if (top_inter.x+rect1.size.width<rect2.top_left.x+rect2.size.width) {
			size_inter.width=rect1.size.width;
		} else {
			size_inter.width=rect2.top_left.x+rect2.size.width-top_inter.x;
		}
	} else {
		top_inter.x=rect1.top_left.x;
		if (rect1.size.width<rect2.size.width) {
			size_inter.width=rect1.size.width;
		} else {
			size_inter.width=rect2.size.width;
		}
	}
	// On prend le max des top_left.y
	if (rect1.top_left.y<rect2.top_left.y) {
		top_inter.y=rect2.top_left.y;
		if (top_inter.y+rect2.size.height<rect1.top_left.y+rect1.size.height) {
			size_inter.height=rect2.size.height;
		} else {
			size_inter.height=rect1.top_left.y+rect1.size.height-top_inter.y;
		}
	} else if (rect1.top_left.y>rect2.top_left.y) {
		top_inter.y=rect1.top_left.y;
		if (top_inter.y+rect1.size.height<rect2.top_left.y+rect2.size.height) {
			size_inter.height=rect1.size.height;
		} else {
			size_inter.height=rect2.top_left.y+rect2.size.height-top_inter.y;
		}
	} else {
		top_inter.y=rect1.top_left.y;
		if (rect1.size.height<rect2.size.height) {
			size_inter.height=rect1.size.height;
		} else {
			size_inter.height=rect2.size.height;
		}
	}
	// Resultat
	if (size_inter.height<0 || size_inter.width<0) {
		return NULL;
	} else {
		ei_rect_t *inter=malloc(sizeof(ei_rect_t));
		inter->top_left=top_inter;
		inter->size=size_inter;
		return inter;
	}
}

void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
	// Initialisation de la surface de texte
	ei_surface_t *surface_text;
	ei_font_t new_font;
	if (font==NULL) {
		new_font=ei_default_font;
	} else {		
		new_font=font;
	}
	surface_text = hw_text_create_surface(text,new_font,color);
	ei_rect_t rect_text = hw_surface_get_rect(surface_text);
	ei_rect_t rect_dest=rect_text;
	rect_dest.top_left = *where;
	if (clipper!=NULL) {
		ei_rect_t *inter_clip=inter_rect(rect_dest,*clipper);
		if (inter_clip!=NULL) {
			ei_rect_t *copy=malloc(sizeof(ei_rect_t));
			*copy=*inter_clip;
			copy->top_left.x=copy->top_left.x-where->x;
			copy->top_left.y=copy->top_left.y-where->y;
			ei_rect_t *tmp=inter_rect(*copy,rect_text);
			rect_text=*tmp;
			rect_dest=*inter_clip;
			free(copy);
		}
	}
	ei_copy_surface(surface,&rect_dest,surface_text,&rect_text,EI_TRUE);
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
	ei_rect_t dst;
	ei_rect_t src;

	if (dst_rect == NULL && src_rect==NULL)	{
		dst = hw_surface_get_rect(destination);
		src = hw_surface_get_rect(source);
	} else if ((src_rect->size.width == dst_rect->size.width) && 
		(src_rect->size.height == dst_rect->size.height)) {
		dst = *dst_rect;
		src = *src_rect;
	} else {
		return 1;
	}
	uint32_t *ptr_src_origin = (uint32_t*) hw_surface_get_buffer(source);
	uint32_t *ptr_dst_origin = (uint32_t*) hw_surface_get_buffer(destination);
	ptr_src_origin =ptr_src_origin + hw_surface_get_size(source).width*src.top_left.y + src.top_left.x;
	ptr_dst_origin =ptr_dst_origin + hw_surface_get_size(destination).width*dst.top_left.y + dst.top_left.x;

	uint32_t *ptr_src = ptr_src_origin;
	uint32_t *ptr_dst = ptr_dst_origin;
	for (uint32_t y = src.top_left.y; y < src.top_left.y + src.size.height; y++) {
		for (uint32_t x = dst.top_left.x; x < dst.top_left.x + dst.size.width; x++) {
			if (alpha == EI_FALSE) {
				ptr_src = ptr_src + 1;
				ptr_dst = ptr_dst + 1;
				*ptr_dst = *ptr_src;
			} else {
				ptr_src = ptr_src + 1;
				ptr_dst = ptr_dst + 1;
				*ptr_dst = alpha_effect(destination,source,ptr_dst,ptr_src);				
			}
		}
		ptr_src = ptr_src + (uint32_t)(hw_surface_get_size(source).width -src.size.width);
		ptr_dst = ptr_dst + (uint32_t)(hw_surface_get_size(destination).width -src.size.width);
	}
	return 0;
}
