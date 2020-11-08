#include "creature.h"

creature::creature(){
	wandering = true;
	new_point = true;
	holding_item = false;
	loc = NULL;
	held_item = NULL;
	scale_factor = 1;
	real_job = true;
}

creature::~creature(){
	clear_travel();
	if (loc != NULL) {
		delete loc;
	}
	new_point = true;
}

void creature::clear_travel() {
	for (size_t i = 0; i < travel_que.size(); i++) {
		delete travel_que.front();
		travel_que.pop();
	}
}

void creature::add_next_nav_point(glm::vec3* i) {
	travel_que.push(i);
	new_point = false;
}

glm::vec3* creature::get_next_nav_point() {
	glm::vec3* output = NULL;
	if (travel_que.size() > 0) {
		output = travel_que.front();
	}
	else {
		new_point = true;
	}

	return output;
}


update_pak* creature::generate_item_update() {
	update_pak* output = NULL;
	if (held_item != NULL) {
		output = new update_pak;
		output->item_id = held_item->item_id;
		output->buffer_loc = held_item->buffer_loc;
		output->x = (loc->x/ scale_factor);
		output->y = (loc->y/ scale_factor)+ 4;
		output->z = (loc->z/ scale_factor);
		output->x_scale = held_item->item_id;
		output->y_scale = held_item->item_id;
		output->z_scale = held_item->item_id;
	}
	return output;
}

void creature::swap_dest_loc() {
	x = dx;
	y = dy;
	z = dz;
}

void creature::map_loc_check() {
	if (x != dx) {
		x = dx;
	}
	if (y != dy) {
		y = dy;
	}
	if (z != dz) {
		z = dz;
	}
}