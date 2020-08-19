#include "world.h"

world::world() {
	update_projection = false;
	update_cam = false;

	//lighting test
	x = 20.0f;
	y = -3.0f;
	z = 0.0f;
	angle = 0;
}

void world::draw() {
	/*
	Sky->set_cam(view);
	Sky->draw();

	Terrian->set_cam(view);
	Terrian->draw();
	
	BM->set_cam(view);
	BM->draw();*/
	glm::mat4 model = glm::mat4(1.0f);
	//the light source
	model = glm::translate(model, glm::vec3(x, y, z));
	def->use();
	def->setMat4("model", model);
	def->setMat4("projection", projection);
	def->setMat4("view", view);
	cube->Draw(def);

	//the obj
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	lighting->use();
	lighting->setMat4("model", model);
	lighting->setMat4("projection", projection);
	lighting->setMat4("view", view);
	lighting->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lighting->setVec3("lightPos", glm::vec3(x, y, z));
	//lighting->setVec3("lightColor", 0.0f, 1.0f, 1.0f);
	lighting->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	moon->Draw(lighting);

}

void world::update(float deltaTime) {
	//Terrian->update(deltaTime);
	//Sky->update(deltaTime);
	//BM->update(deltaTime);

	x = 0 + 30 * cos(angle);
	y = -3 + 30 * sin(angle);

	angle += (angle_incr * deltaTime);
	if (angle >= 360) {
		angle = 0;
	}

}

void world::init() {
	//lighting test init
	moon = new Model("resources/objects/planet/planet.obj");
	cube = new Model("resources/objects/cube/cube.obj");
	lighting = new Shader("lighting.vs", "lighting.fs");
	def = new Shader("planet.vs", "planet.fs");

	Terrian = new terrian();
	Terrian->set_projection(projection);
	Terrian->set_cam(view);
	//Terrian->cubes_init();

	BM = new beast_manager();
	BM->set_projection(projection);
	BM->set_cam(view);
	BM->set_terrian(Terrian);
	BM->set_map_size(Terrian->get_x_width(), Terrian->get_z_width());
	//BM->init();

	Sky = new sky();
	Sky->set_projection(projection);
	Sky->set_cam(view);
	Sky->set_width(Terrian->get_x_width(), Terrian->get_y_width(), Terrian->get_z_width());
	//Sky->init();
}