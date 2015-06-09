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
		//printf("droite entre ( %u ; %u ) et ( %u ; %u).",x1,y1,x2,y2);
		dx=x2-x1;
		if ( dx != 0 ){
			if ( dx > 0 ){
				dy = y2 - y1;
				if ( dy != 0 ){
					if ( dy > 0 ){
						//Vecteur oblique 1er cadran
						if ( dx >= dy ){
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
								if (e < 0) {
									x1=x1+1;
									e=e+dy;
								}
							}
						}
					} else {
						//Vecteur oblique 4eme cadran
						if ( dx + dy >= 0 ){
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
								if ( e < 0 ) {
									y1 = y1 - 1;
									e = e + dx;
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
								if ( e > 0 ) {
									x1 = x1 + 1;
									e = e + dy;
								}
							}
						}
					}
				} else {
					//Vecteur horizontal vers la droite
					while ( x1 != x2 ){
						draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
						x1 = x1 + 1;
					}
				
				}
			} else {
				dy = y2 - y1;
				if ( dy != 0 ){
					if ( dy > 0 ){
						//2e cadran
						if ( dx + dy <= 0 ){
							//4e octant
							e = dx;
							dx = 2*e;
							dy = 2*dy;

							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1-1;
								e=e+dy;
								if ( e >= 0 ) {
									y1 = y1 + 1;
									e = e + dx;
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
								if ( e <= 0 ) {
									x1 = x1 - 1;
									e = e + dy;
								}
							}	
						}
					} else {
						//3e cadran
						if ( dx <= dy ){
							//5e octant
							e = dx;
							dx = 2*e;
							dy = 2*dy;

							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (x1!=x2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								x1=x1-1;
								e=e-dy;
								if ( e >= 0 ) {
									y1 = y1 - 1;
									e = e + dx;
								}
							}
						} else {
							//6e octant
							e = dy;
							dy = 2*e;
							dx = 2*dx;

							/* Boucle de traitement, segment par segment */
							/* Traitement */
							while (y1!=y2) {
								draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
								y1=y1-1;
								e=e-dx;
								if ( e >= 0 ) {
									x1 = x1 - 1;
									e = e + dy;
								}
							}
						}
					} 
				} else {
					//Vecteur horizontal vers la gauche
					while ( x1 != x2 ){
						draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
						x1 = x1 - 1;
					}
				}
			}
		} else {
			dy = y2 - y1;
			if ( dy > 0 ){
				//Vecteur vertical croissant
				while ( y1 != y2 ){
					draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
					y1 = y1 + 1;
				}
			} else {
				//Vecteur vertical decroissant
				while ( y1 != y2 ){
					draw_pixel(surface,x1,y1,color,pixel_ptr,clipper);
					y1 = y1 - 1;
				}
			}
		}
		/* Dessiner dans le dernier pixel qui n'est pas pris en compte dans la boucle */
		draw_pixel(surface,x2,y2,color,pixel_ptr,clipper);
		
		/* sent->next != NULL */
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
		//float pente_rec;
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
				//new_side->begin=p1;
				//new_side->end=p2;
				new_side->next=NULL;
				// On remplit TC avec le nouveau côté
				if (TC[num_scan]!=NULL) {
					ei_side_t *sent_tab=TC[num_scan];
					while (sent_tab->next!=NULL) {
						sent_tab=sent_tab->next;               
					}					
					sent_tab->next=new_side;
					//printf("ajout dans %d\n",num_scan);
				} else {
					TC[num_scan]=new_side;
					//printf("premier dans %d\n",num_scan);
				}
			}		
			// On change de coté
			sent=sent->next;
		}
		//free(sent);
		// Verification de TC 
		/*
		  for (int j=0; j<size.height; j++) {
		  printf("Scan numero : %d\n",j);
		  ei_side_t *sent_verif=TC[j];
		  while (sent_verif!=NULL) {
		  printf("ymax : %d xymin : %d\n",sent_verif->ymax, sent_verif->x_ymin);
		  sent_verif=sent_verif->next;
		  }
		  }
		*/

		// Recuperer l'adresse du pixel (0,0)
		uint32_t* pixel_ptr=(uint32_t*)hw_surface_get_buffer(surface);

		//Creation de la TCA
		TCA *TCA=create_TCA();

		// Boucle des scanlines
		for (int k=0; k<size.height; k++) {
			ei_side_t *sent_tab=TC[k];
			/* printf("Scan numero : %d\n", k); */
			while (sent_tab!=NULL) {
				// A FAIRE : Supression dans TC
				// A FAIRE : Gérer la libération 
				// Ajout en tete de TCA 
				add_TCA(TCA,*sent_tab);	      
				//printf("c");
				sent_tab=sent_tab->next;
			}
			//free(TC[k]);
			// On supprime les éléments tels que y_max=k
			del_TCA(TCA,k);
			// On trie la TCA 
			TCA=sort_TCA(TCA);		

			// Verification de TCA a chaque scanline
			/*
			  cell_TCA *sent_TCA2=TCA->head;
			  while (sent_TCA2!=NULL) {
			  printf("ymax : %d xymin : %d x_inter : %f\n",sent_TCA2->side.ymax, sent_TCA2->side.x_ymin, sent_TCA2->x_inter);
			  sent_TCA2=sent_TCA2->next;
			  }
			*/

			// Remplissage de la scanline
			cell_TCA *senti_TCA=TCA->head;
			while (senti_TCA!=NULL && senti_TCA->next != NULL) {
				float inter1=ceilf(senti_TCA->x_inter);
				float inter2=floorf(senti_TCA->next->x_inter);
				// Gestion des règles de remplissage
				int inf=(int) inter1;
				int sup=(int) inter2;
				/* printf("inf : %d sup : %d\n",inf,sup); */
				// Dessin entre deux intersections successives intersections
				for (int m=inf; m<=sup; m++) {
					//printf("Dessin en %d %d\n",m,k);
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
				//if (k!=size.height-1) {
					// Mise a jour des intersections
					/*
					// Initialisation des variables
					int x1=maj_x_inter->side.begin.x;
					int x2=maj_x_inter->side.end.x;
					int y1=maj_x_inter->side.begin.y;
					int y2=maj_x_inter->side.end.y;
					int dx=x2-x1;
					int dy=y2-y1;
					int e;
					*/
					/*
					// On réutilise Bresenham
					if (dx !=0) {
						if (dx>0) {
							if (dy!=0) {
								if (dy>0) {
									// Vecteur oblique 1er cadran
									if (dx>=dy) {
										// 1er octant
										e=dx;
										dx=2*e;
										dy=2*dy;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
											x1=x1+1;
											e=e+dy;
											if (2*e>dx) {
												y1=y1+1;
												e=e-dx;
											}
										}
									} else {
										// 2eme octant
										e=dy;
										dx=2*dx;
										dy=2*e;
										// Boucle de traitement, segment par segment
										// Traitement 
										while (y1!=k+1) {
											y1=y1+1;
											e=e-dx;
											if (e < 0) {
												x1=x1+1;
												e=e+dy;
											}
										}
									}
								} else {
									// Vecteur oblique 4eme cadran
									if (dx+dy>=0){
										//8e octant
										e=dx;
										dx=2*e;
										dy=2*dy;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
											x1=x1+1;
											e=e+dy;
											if (e<0) {
												y1=y1-1;
												e=e+dx;
											}
										}
									} else {
										// 7e octant
										e=dy;
										dx=2*dx;
										dy=2*e;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
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
								// Vecteur horizontal vers la droite
								// Cas impossible puisqu'on ne prend pas en compte les segments horizontaux
								while (y1!=k){
								  x1=x1+1;
								  }								
								printf("Segment horizontal\n");
				
							}
						} else {
							if (dy!=0){
								if (dy>0){
									// 2e cadran
									if (dx+dy<=0){
										// 4e octant
										e=dx;
										dx=2*e;
										dy=2*dy;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
											x1=x1-1;
											e=e+dy;
											if (e>=0) {
												y1=y1+1;
												e=e+dx;
											}
										}
									} else {
										// 3e octant
										e=dy;
										dx=2*dx;
										dy=2*e;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
											y1=y1+1;
											e=e+dx;
											if (e<=0) {
												x1=x1-1;
												e=e+dy;
											}
										}	
									}
								} else {
									// 3e cadran
									if (dx<=dy){
										// 5e octant
										e=dx;
										dx=2*e;
										dy=2*dy;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
											x1=x1-1;
											e=e-dy;
											if (e>=0) {
												y1=y1-1;
												e = e+dx;
											}
										}
									} else {
										// 6e octant
										e=dy;
										dy=2*e;
										dx=2*dx;
										// Boucle de traitement, segment par segment
										// Traitement
										while (y1!=k+1) {
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
								// Vecteur horizontal vers la gauche
								while (y1!=k+1){
									x1=x1-1;
								}
							}
						}
					} else {
						if (dy>0){
							// Vecteur vertical croissant
							while (y1!=k+1){
								y1=y1+1;
							}
						} else {
							// Vecteur vertical decroissant
							while (y1!=k+1){
								y1=y1-1;
							}
						}
					}*/			       
					//}			  
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
	//clipper null ou contenant le texte
	ei_surface_t *surface_text;
	surface_text = hw_text_create_surface(text,font,color);
	ei_rect_t rect_text = hw_surface_get_rect(surface_text);
	ei_rect_t rect_dest=rect_text;
	rect_dest.top_left = *where;
	// if (clipper == NULL || (clipper->top_left.x < where->x
	// 	&& clipper->top_left.y < where->y
	// 	&& clipper->size.width > rect_text.size.width 
	// 	&& clipper->size.height > rect_text.size.height))
	// {

	// 	rect_dest = rect_text;
	// 	rect_dest.top_left = *where;
	// }
	// //le topleft du clipper est dans rect_text
	// else if (where->x > clipper->top_left.x && where->y > clipper->top_left.y)
	// {
	// 	rect_dest.top_left = clipper->top_left;
	// 	//cas clipper dans rect_text en entier
	// 	if (((rect_text.size.width - (clipper->top_left.x - where->x)) > clipper->size.width)
	// 	&& ((rect_text.size.height - (clipper->top_left.y - where->y)) > clipper->size.height))
	// 	{
	// 		rect_dest = *clipper;
	// 	}
	// 	//cas du clipper depassant rect_text en largeur
	// 	else if ((rect_text.size.width - (clipper->top_left.x - where->x)) < clipper->size.width 
	// 			&& (rect_text.size.height - (clipper->top_left.y - where->y)) > clipper->size.height)
	// 	{
	// 		rect_dest.size.width = clipper->size.width;
	// 		rect_dest.size.height = rect_text.size.height; 
	// 	}
	// 	//cas du clipper depassant rect_text en hauteur
	// 	else if ((rect_text.size.width - (clipper->top_left.x - where->x)) > clipper->size.width 
	// 			&& (rect_text.size.height - (clipper->top_left.y - where->y)) < clipper->size.height)
	// 	{
	// 		rect_dest.size.height = clipper->size.height;
	// 		rect_dest.size.width = rect_text.size.width; 
	// 	}
	// 	//le clipper depasse rect_text en largeur et hauteur
	// 	else if ((rect_text.size.width - (clipper->top_left.x - where->x)) < clipper->size.width 
	// 			&& (rect_text.size.height - (clipper->top_left.y - where->y)) < clipper->size.height)
	// 	{
	// 		rect_dest.size = clipper->size;
	// 	}
	// }
	// //le topleft du clipper est en dehors du rect_text
	// 	//northwest
	// else if (clipper->top_left.x < where->x
	// 		&& clipper->top_left.y < where->y)	
	// {
	// 	//rect_text en entier dans clipper;
	// 	if (clipper->size.width + clipper->top_left.x > where->x + rect_text.size.width
	// 		&& clipper->size.height + clipper->top_left.y > where->y + rect_text.size.height)
	// 	{
	// 		rect_dest = rect_text;
	// 	}
	// 	//les cas restants : on prend toujours le clipper
	// 	else
	// 	{
	// 		rect_dest = *clipper;
	// 	}
	// }
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

	if (dst_rect == NULL && src_rect==NULL)
	{
		dst = hw_surface_get_rect(destination);
		src = hw_surface_get_rect(source);
	}
	else if ((src_rect->size.width == dst_rect->size.width) && 
		(src_rect->size.height == dst_rect->size.height))
	{
		dst = *dst_rect;
		src = *src_rect;
	}
	else
	{
		return 1;
	}
	uint32_t *ptr_src_origin = (uint32_t*) hw_surface_get_buffer(source);
	uint32_t *ptr_dst_origin = (uint32_t*) hw_surface_get_buffer(destination);
	ptr_src_origin =ptr_src_origin + src.size.width*src.top_left.y + src.top_left.x;
	ptr_dst_origin =ptr_dst_origin + hw_surface_get_size(destination).width*dst.top_left.y + dst.top_left.x;

	uint32_t *ptr_src = ptr_src_origin;
	uint32_t *ptr_dst = ptr_dst_origin;

	for (uint32_t y = src.top_left.y; y < src.top_left.y + src.size.height; y++)
	{
		for (uint32_t x = src.top_left.x; x < src.top_left.x + src.size.width; x++)
		{
			if (alpha == EI_FALSE)
			{
				ptr_src = ptr_src + 1;
				ptr_dst = ptr_dst + 1;
				*ptr_dst = *ptr_src;
			}
			else
			{
				ptr_src = ptr_src + 1;
				ptr_dst = ptr_dst + 1;
				*ptr_dst = alpha_effect(destination,source,ptr_dst,ptr_src);
			}
		}
		ptr_dst = ptr_dst + (uint32_t)(hw_surface_get_size(destination).width - src.size.width);
	}
	return 0;
}
