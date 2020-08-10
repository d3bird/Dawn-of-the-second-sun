#include "creature.h"

creature::creature(){
	wandering = true;
}

creature::~creature(){
	clear_travel();
}

void creature::clear_travel() {
	for (size_t i = 0; i < travel_que.size(); i++) {
		delete travel_que.front();
		travel_que.pop();
	}
}