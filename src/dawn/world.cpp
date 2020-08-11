#include "world.h"

world::world() {
	update_projection = false;
	update_cam = false;
}

void world::draw() {
	Terrian->set_cam(view);
	Terrian->draw();
	
	BM->set_cam(view);
	BM->draw();
}

void world::update(float deltaTime) {
	//Terrian->update(deltaTime);

	BM->update(deltaTime);
}

void world::init() {
	Terrian = new terrian();
	Terrian->set_projection(projection);
	Terrian->set_cam(view);
	Terrian->cubes_init();

	BM = new beast_manager();
	BM->set_projection(projection);
	BM->set_cam(view);
	BM->set_map(Terrian->get_map());
	BM->set_map_size(Terrian->get_x_width(), Terrian->get_z_width());
	BM->init();
}