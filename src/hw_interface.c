#include "hw_interface.h"
#include "ei_types.h"

#include <stdint.h>

void hw_init(){
	
}

void hw_quit(){
	
}

ei_surface_t hw_create_window(ei_size_t* size, const ei_bool_t fullScreen);

ei_surface_t hw_surface_create (const ei_surface_t	root,
				const ei_size_t*	size,
				ei_bool_t		force_alpha);

void hw_surface_free(ei_surface_t surface){

}

void hw_surface_lock(ei_surface_t surface){

}

void hw_surface_unlock(ei_surface_t surface){

}

void hw_surface_update_rects(ei_surface_t surface, const ei_linked_rect_t* rects){

}

void hw_surface_get_channel_indices(ei_surface_t surface, int* ir, int* ig, int* ib, int* ia){

}

void hw_surface_set_origin(ei_surface_t surface, const ei_point_t origin){

}

uint8_t* hw_surface_get_buffer(const ei_surface_t surface){

}

ei_size_t hw_surface_get_size(const ei_surface_t surface){

}

ei_rect_t hw_surface_get_rect(const ei_surface_t surface){

}

ei_bool_t hw_surface_has_alpha(ei_surface_t surface){

}

ei_font_t hw_text_font_create		(const char*		filename,
					 ei_fontstyle_t		style,
					 int			size){

}

void hw_text_font_free(ei_font_t font){

void hw_text_compute_size		(const char*		text,
					 const ei_font_t	font,
					 int*			width,
					 int*			height){

}

ei_surface_t hw_text_create_surface	(const char*		text,
					 const ei_font_t	font,
					 const ei_color_t*	color){

}

ei_surface_t hw_image_load		(const char*		filename,
					 ei_surface_t		channels){

}

/* Gestion événements */
void hw_event_wait_next(struct ei_event_t* event){

}

int hw_event_post_app(void* user_param){

}

void hw_event_schedule_app(int ms_delay, void* user_param){

}

double hw_now(){

}
