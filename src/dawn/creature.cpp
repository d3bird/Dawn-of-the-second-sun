#include "creature.h"

creature::creature(){
	wandering = true;
	new_point = true;
	loc = NULL;
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