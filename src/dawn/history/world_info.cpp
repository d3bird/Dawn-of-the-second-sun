#include "world_info.h"


world_info::world_info() {
	hist = NULL;
	World_map = NULL;
}

void world_info::draw() {
	World_map->set_projection(projection);
	World_map->draw();
}

void world_info::update(float deltaTime) {
	World_map->update(deltaTime);
}

void world_info::init() {
	std::cout << "generating world" << std::endl;

	next_line();

	hist = new history();
	hist->set_projection(projection);
	hist->set_cam(view);
	hist->init();
	next_line();

	World_map = new world_map();
	World_map->set_projection(projection);
	World_map->set_cam(view);
	World_map->init();
	next_line();

	std::cout << "finished generating world" << std::endl;
}