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
	while (sent->next!=NULL) {
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
	// A FAIRE : Gérer le problème du polygone nul
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
	int y_max,x_ymin;
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
				x_ymin=p1.x;
				num_scan=p1.y;
			} else {
				y_max=p1.y;
				x_ymin=p2.x;
				num_scan=p2.y;
			}
			// On remplit TC avec les informations que l'on a 
			//ei_side_t *sent_tab=TC[num_scan];
			if (TC[num_scan]!=NULL) {
				ei_side_t *sent_tab=TC[num_scan];
				while (sent_tab->next!=NULL) {
					sent_tab=sent_tab->next;               
				}
				// Création du nouveau coté dans TC
				ei_side_t *new_side=malloc(sizeof(ei_side_t));
				assert(new_side!=NULL);
				new_side->ymax=y_max;
				new_side->x_ymin=x_ymin;
				new_side->begin=p1;
			        new_side->end=p2;
				new_side->next=NULL;
				sent_tab->next=new_side;
				//printf("ajout dans %d\n",num_scan);
			} else {
				// Création du nouveau coté dans TC
				ei_side_t *new_side=malloc(sizeof(ei_side_t));
				assert(new_side!=NULL);
				new_side->ymax=y_max;
				new_side->x_ymin=x_ymin;
				new_side->begin=p1;
				new_side->end=p2;
				new_side->next=NULL;
				TC[num_scan]=new_side;
				//printf("premier dans %d\n",num_scan);
			}
		}		
		// On change de coté
		sent=sent->next;
	}

	/* Verification */ 
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
		printf("Scan numero : %d\n", k);
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

		// Verification 
		// Affichage de TCA par scanline 
		cell_TCA *sent_TCA2=TCA->head;
		while (sent_TCA2!=NULL) {
			printf("ymax : %d xymin : %d x_inter : %d\n",sent_TCA2->side.ymax, sent_TCA2->side.x_ymin, sent_TCA2->x_inter);
			sent_TCA2=sent_TCA2->next;
		}

		// Remplissage de la scanline
		cell_TCA *senti_TCA=TCA->head;
		while (senti_TCA!=NULL) {
			int inter1=senti_TCA->x_inter;
			int inter2=senti_TCA->next->x_inter;
			// Dessin entre deux intersections successives intersections
			for (int m=inter1; m<=inter2; m++) {
				// A FAIRE : Gerer les règles de remplissage
				draw_pixel(surface,m,k,color,pixel_ptr,clipper);
			}
			senti_TCA=senti_TCA->next->next;
		}
		cell_TCA *maj_x_inter=TCA->head;
		while (maj_x_inter!=NULL) {
			// Mise a jour des intersections
			// Initialisation des variables
			int x1=maj_x_inter->side.begin.x;
			int x2=maj_x_inter->side.end.x;
			int y1=maj_x_inter->side.begin.y;
			int y2=maj_x_inter->side.end.y;
			int dx=x2-x1;
			int dy=y2-y1;
			int e;
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
								while (y1!=k) {
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
								while (x1!=maj_x_inter->x_inter && y1!=k) {
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
							if ( dx + dy >= 0 ){
								//8e octant
								e=dx;
								dx=2*e;
								dy=2*dy;
								// Boucle de traitement, segment par segment
								// Traitement
								while (x1!=maj_x_inter->x_inter && y1!=k) {
									x1=x1+1;
									e=e+dy;
									if ( e < 0 ) {
										y1 = y1 - 1;
										e = e + dx;
									}
								}
							} else {
								// 7e octant
								e=dy;
								dx=2*dx;
								dy=2*e;
								// Boucle de traitement, segment par segment
								// Traitement
								while (x1!=maj_x_inter->x_inter && y1!=k) {
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
						// Vecteur horizontal vers la droite
						while (x1!=maj_x_inter->x_inter && y1!=k){
							x1 = x1 + 1;
						}
				
					}
				} else {
					if ( dy != 0 ){
						if ( dy > 0 ){
							// 2e cadran
							if ( dx + dy <= 0 ){
								// 4e octant
								e = dx;
								dx = 2*e;
								dy = 2*dy;
								// Boucle de traitement, segment par segment
								// Traitement
								while (y1!=k) {
									x1=x1-1;
									e=e+dy;
									if ( e >= 0 ) {
										y1 = y1 + 1;
										e = e + dx;
									}
								}
							} else {
								// 3e octant
								e = dy;
								dx = 2*dx;
								dy = 2*e;
								// Boucle de traitement, segment par segment
								// Traitement
								while (y1!=k) {
									y1=y1+1;
									e=e+dx;
									if ( e <= 0 ) {
										x1 = x1 - 1;
										e = e + dy;
									}
								}	
							}
						} else {
							// 3e cadran
							if ( dx <= dy ){
								// 5e octant
								e = dx;
								dx = 2*e;
								dy = 2*dy;
								// Boucle de traitement, segment par segment
								// Traitement
								while (x1!=maj_x_inter->x_inter && y1!=k) {
									x1=x1-1;
									e=e-dy;
									if ( e >= 0 ) {
										y1 = y1 - 1;
										e = e + dx;
									}
								}
							} else {
								// 6e octant
								e = dy;
								dy = 2*e;
								dx = 2*dx;
								// Boucle de traitement, segment par segment
								// Traitement
								while (x1!=maj_x_inter->x_inter && y1!=k) {
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
						// Vecteur horizontal vers la gauche
						while (x1!=maj_x_inter->x_inter && y1!=k){
							x1 = x1 - 1;
						}
					}
				}
			} else {
				if ( dy > 0 ){
					// Vecteur vertical croissant
					while (x1!=maj_x_inter->x_inter && y1!=k){
						y1 = y1 + 1;
					}
				} else {
					// Vecteur vertical decroissant
					while (x1!=maj_x_inter->x_inter && y1!=k){
						y1 = y1 - 1;
					}
				}
			}
			maj_x_inter->x_inter=x1;
			maj_x_inter=maj_x_inter->next;
		}
		//maj_x_inter->x_inter=x1;
	}

}

void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
	ei_surface_t *surface_text;
	int *h_t,*w_t;
	//if (font == NULL)
	//{
	surface_text = hw_text_create_surface(text,ei_default_font,color);
	hw_text_compute_size(text,ei_default_font,w_t,h_t);
	ei_rect_t rect_text = hw_surface_get_rect(surface_text);
	ei_rect_t rect_source = rect_text;
	rect_source.top_left = *where;
	//}
	ei_bool_t alpha;
	if (hw_surface_has_alpha(surface)==EI_TRUE)
	{
		alpha = EI_TRUE;
	}
	else
	{
		alpha=EI_FALSE;
	}
	ei_copy_surface(surface,&rect_source,surface_text,&rect_text,alpha);

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
	uint32_t *px_source_origin =(uint32_t*)hw_surface_get_buffer(source);
	uint32_t *px_dest_origin =(uint32_t*)hw_surface_get_buffer(destination);

	uint32_t *px_source;
	uint32_t *px_dest ;

	ei_size_t taille_source;
	ei_size_t taille_dest;
	if (dst_rect==NULL && src_rect==NULL)
	{
			taille_source.width = hw_surface_get_size(source).width;
			taille_source.height = hw_surface_get_size(source).height;

			taille_dest.width = hw_surface_get_size(destination).width;
			taille_dest.height = hw_surface_get_size(destination).height;

	}
	else if ((dst_rect->size.width!=src_rect->size.width) && 
		(dst_rect->size.height!=src_rect->size.height))
	{
			return 1;
	}
	else if (dst_rect==NULL && src_rect!=NULL)
	{
		return 1;
	}
	else if (dst_rect!=NULL && src_rect==NULL)
	{
		return 1;
	}
	else
	{
		taille_source.width = src_rect->size.width;
		taille_source.height = src_rect->size.height;

		taille_dest.width = dst_rect->size.width;
		taille_dest.height = dst_rect->size.height;

		*px_source_origin ++= src_rect->size.width*src_rect->top_left.y+src_rect->top_left.x;
		*px_dest_origin ++= dst_rect->size.width*dst_rect->top_left.y+dst_rect->top_left.x;
	}
	if(taille_source.height > taille_dest.height)
	{
		for(int y = 0; y<= taille_dest.height; y++)
		{
			if (taille_source.width > taille_dest.width)
			{
				for (int x = 0; x<= taille_dest.width; x++)
				{
					px_source = px_source_origin + y*taille_dest.width + x;
					px_dest = px_dest_origin + y*taille_dest.width + x;
					if(alpha==EI_FALSE)
					{
						*px_dest = *px_source;
					}
					else
					{
						*px_dest = alpha_effect(source,destination,px_source,px_dest);
					}
				}
			}
			else
			{
				for (int x = 0; x<= taille_source.width; x++)
				{
					px_source = px_source_origin + y*taille_dest.width + x;
					px_dest = px_dest_origin + y*taille_dest.width + x;
					if(alpha==EI_FALSE)
					{
						*px_dest = *px_source;
					}
					else
					{
						*px_dest = alpha_effect(source,destination,px_source,px_dest);
					}
				}
			}
		}
	}
	else
	{
		for(int y = 0; y<= taille_source.height; y++)
		{
		if (taille_source.width > taille_dest.width)
			{
				for (int x = 0; x<= taille_dest.width; x++)
				{
					px_source = px_source_origin + y*taille_source.width + x;
					px_dest = px_dest_origin + y*taille_source.width + x;
					if(alpha==EI_FALSE)
					{
						*px_dest = *px_source;
					}
					else
					{
						*px_dest = alpha_effect(source,destination,px_source,px_dest);
					}
				}
			}
			else
			{
				for (int x = 0; x<= taille_source.width; x++)
				{
					px_source = px_source_origin + y*taille_source.width + x;
					px_dest = px_dest_origin + y*taille_source.width + x;
					if(alpha==EI_FALSE)
					{
						*px_dest = *px_source;
					}
					else
					{
						*px_dest = alpha_effect(source,destination,px_source,px_dest);
					}
				}
			}	
		}
	}
	return 0;
}
