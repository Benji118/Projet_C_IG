#include "geometry.h"
#include "ei_draw.h"
#include "ei_types.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

static  void ajoute_en_tete(int abs, int ord, ei_linked_point_t **liste){
	ei_linked_point_t *new_point = malloc(sizeof(ei_linked_point_t));
	assert(new_point != NULL);

	new_point->point.x = abs;
	new_point->point.y = ord;
	new_point->next = *liste;

	*liste = new_point;
}

static void ajoute_octant(uint8_t octant, ei_linked_point_t **arc, double rayon, ei_point_t centre){
	
	/* Algorithme de Bresenham pour l'arc de cercle */
	int x = 0;
	int y = rayon;
	int32_t m = 5 - 4*rayon;
	ei_linked_point_t **octant_courant = malloc(sizeof(ei_linked_point_t *));
	*octant_courant = NULL;

	while ( x<=y ){
		switch(octant){
		case 1:
			ajoute_en_tete(  y + centre.x,  x + centre.y, arc);
			break;
		case 2:
			ajoute_en_tete(  x + centre.x,  y + centre.y, octant_courant);
			break;
		case 3:
			ajoute_en_tete( -x + centre.x,  y + centre.y, arc);
			break;
		case 4:
			ajoute_en_tete( -y + centre.x,  x + centre.y, octant_courant);
			break;
		case 5:
			ajoute_en_tete( -y + centre.x, -x + centre.y, arc);
			break;
		case 6:
			ajoute_en_tete( -x + centre.x, -y + centre.y, octant_courant);
			break;
		case 7:
			ajoute_en_tete(  x + centre.x, -y + centre.y, arc);
			break;
		case 8:
			ajoute_en_tete(  y + centre.x,  -x + centre.y, octant_courant);
			break;
		default:
			break;
		}
		if ( m > 0 ){
			y = y - 1;
			m = m - 8*y;
		}
		x = x + 1;
		m = m + 8*x + 4;
	}

	/* Pour parcourir le cercle continument on inverse le parcours d'un octant sur deux */
	if (octant == 2 || octant == 4 || octant == 6 || octant == 8){
		while (*octant_courant != NULL){
			ajoute_en_tete((*octant_courant)->point.x, (*octant_courant)->point.y, arc);
			*octant_courant = (*octant_courant)->next;
		}
	}
}

ei_linked_point_t *arc(double rayon, ei_point_t centre, double depart, double arrivee){
	ei_linked_point_t **arc = malloc(sizeof(ei_linked_point_t*));
	*arc = NULL;
	
	/* On regarde quels octants touchent l'arc */
	/* Résultat placé dans un tableau de booléens */
	bool octant_present[8];
	uint8_t i=0;

	/* On se place dans [0,360[ */
	while ( depart >= 360.0 )
		depart -= 360.0;
	
	while ( depart < 0)
		depart += 360.0;
	
	while ( arrivee > 360.0 )
		arrivee -= 360.0;
	
	while ( arrivee < 0)
		arrivee += 360.0;
	

	/* Les octants qui terminent avant le depart ne sont pas dans l'arc */
	while ( (i+1)*45 <= depart && i<8 ){
		octant_present[i] = false;
		i++;
	}
	
	/* Les octants qui terminent avant l'arrivee sont dans l'arc */
	/* (on est après depart) */
	while ( (i)*45 < arrivee && i<8 ){
		octant_present[i] = true;
		i++;
	}

	/* Les octants suivants ne sont pas dans l'arc */
	for (uint8_t k=i; k<8; k++)
		octant_present[k] = false;
	
	
	/* Seuls les points des octants touchés sont vraiment ajoutés */
	for (uint8_t i=0; i<8; i++){
		if (octant_present[i])
			ajoute_octant(i+1, arc, rayon, centre);
		
	}
	
	return *arc;
}

ei_linked_point_t *rounded_frame(ei_rect_t rectangle,
				 double rayon,
				 bool top,
				 bool bottom)
{
	//Requiert top ou bottom 
	ei_point_t centre;
	ei_linked_point_t *coin = NULL;

       	ei_linked_point_t **frame = malloc(sizeof(ei_linked_point_t*));
	*frame = NULL;

	/* On définit les points du milieu */
	double h;
	ei_point_t mid1, mid2;
	if (rectangle.size.height < rectangle.size.width)
		h = rectangle.size.height/2;
	else
		h = rectangle.size.width/2;
	mid1.x = rectangle.top_left.x + h;
	mid1.y = rectangle.top_left.y + h;
	mid2.x = rectangle.top_left.x + rectangle.size.width - h;
	mid2.y = rectangle.top_left.y + rectangle.size.height - h;

	/* Coin sup gauche */
	if ( top ){
		centre.x = rectangle.top_left.x + rayon;
		centre.y = rectangle.top_left.y + rayon;
		coin = arc(rayon, centre, 180.0, 270.0);
		*frame = coin;
	} else 
		ajoute_en_tete(mid2.x, mid2.y, frame);

	/* Coin sup droit */
	centre.x = rectangle.top_left.x + rectangle.size.width - rayon;
	centre.y = rectangle.top_left.y + rayon;
	if ( top ){
		if ( bottom )
			coin = arc(rayon, centre, 270.0, 360.0);
		else {
			coin = arc(rayon, centre, 270.0, 315.0);
		}
	} else {
		coin = arc(rayon, centre, 315.0, 360.0);
	}
	ei_linked_point_t *courant = coin;
	
	/* On relie ce coin au cadre commencé */
	while(courant->next != NULL)
		courant = courant->next;
	*courant = **frame;
	*frame = coin;
	if ( top && (bottom == false)) {
		ajoute_en_tete(mid2.x, mid2.y, frame);
		ajoute_en_tete(mid1.x, mid1.y, frame);
	}

	/* Coin inf droit */
	if ( bottom ){
		centre.x = rectangle.top_left.x + rectangle.size.width - rayon;
		centre.y = rectangle.top_left.y + rectangle.size.height - rayon;
		coin = arc(rayon, centre, 0.0, 90.0);
		courant = coin;
	
		/* On relie ce coin au cadre commencé */
		while(courant->next != NULL)
			courant = courant->next;
		*courant = **frame;
		*frame = coin;
	}

	/* Coin inf gauche */
	centre.x = rectangle.top_left.x + rayon;
	centre.y = rectangle.top_left.y + rectangle.size.height - rayon;
	if ( bottom ){
		if ( top )
			coin = arc(rayon, centre, 90.0, 180.0);
		else 
			coin = arc(rayon, centre, 90.0, 135.0);
	} else
	       	coin = arc(rayon, centre, 135.0, 180.0);
	courant = coin;
	
	/* On relie ce coin au cadre commencé */
	while(courant->next != NULL)
		courant = courant->next;
	*courant = **frame;
	*frame = coin;

	if ( bottom && (!top)){
		ajoute_en_tete(mid1.x, mid1.y, frame);
	}
	/* On rajoute en tête le dernier point pour boucler */
	if ( top )
		ajoute_en_tete(rectangle.top_left.x, rectangle.top_left.y + rayon, frame);
	if ( bottom && (!top))
		ajoute_en_tete(mid2.x, mid2.y, frame);
	return *frame;	
}

void                    draw_button        ( ei_surface_t                   surface,
					     const ei_rect_t                rectangle,
					     const double                   rayon,
					     const double                   edge,
					     const ei_color_t               color,
					     const ei_relief_t              relief,
					     const ei_rect_t*               clipper)
{
	ei_linked_point_t *first_point_top = rounded_frame(rectangle, rayon, true, false);
	ei_linked_point_t *first_point_bot = rounded_frame(rectangle, rayon, false, true);
	
	/* On définit l'ombre et la lumière a partir de la couleur voulue pour le bouton */
	ei_color_t light, shadow;
	light.red = 2*color.red;
	if (light.red > 255)
		light.red = 255;
	light.green = 2*color.green;
	if (light.green > 255)
		light.green = 255;
	light.blue = 2*color.blue;
	if (light.blue > 255)
		light.blue = 255;
	light.alpha = color.alpha;


	shadow.red = color.red/2;
	shadow.green =color.green/2;
	shadow.blue = color.blue/2;
	shadow.alpha = color.alpha;
	

	/* Rectangle principal (dessus du bouton) */
	/* Avec décalage si le bouton est enfoncé */
	ei_rect_t mid_rect;
	if ( relief == ei_relief_sunken ){
		mid_rect.top_left.x = rectangle.top_left.x + (4/3)*edge;
		mid_rect.top_left.y = rectangle.top_left.y + (4/3)*edge;
	} else {
		mid_rect.top_left.x = rectangle.top_left.x + edge;
		mid_rect.top_left.y = rectangle.top_left.y + edge;
	}
	mid_rect.size.width = rectangle.size.width - 2*edge;
	mid_rect.size.height = rectangle.size.height - 2*edge;
	double mid_ray = rayon - edge;

	/* Couleurs choisies selon le type de relief */
	ei_color_t color_top, color_bot;
	if ( relief == ei_relief_sunken ){
		color_top = shadow;
		color_bot = light;
	} else {
		color_top = light;
		color_bot = shadow;
	}
		

	ei_linked_point_t *first_point_main = rounded_frame(mid_rect, mid_ray, true, true);
	ei_draw_polygon(surface, first_point_top,  color_top, clipper);
	ei_draw_polygon(surface, first_point_bot,  color_bot, clipper);
	ei_draw_polygon(surface, first_point_main,  color, clipper);
}
