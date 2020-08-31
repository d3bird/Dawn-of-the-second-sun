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
	
	Sky->set_cam(view);
	Sky->draw();

	lighting_in->use();
	lighting_in->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lighting_in->setVec3("lightPos", Sky->get_light_loc());
	//lighting_in->setVec3("lightColor", 0.0f, 1.0f, 1.0f);
	lighting_in->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	
	Terrian->set_cam(view);
	Terrian->draw();
	
	BM->set_cam(view);
	BM->draw();

	OBJM->draw();
}

void world::draw_selection() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Terrian->set_cam(view);
	Terrian->draw_selection(selection);
	glFlush();
}

void world::update(float deltaTime) {
	//Terrian->update(deltaTime);
	Sky->update(deltaTime);
	BM->update(deltaTime);
	//OBJM->update(deltaTime);
}

void world::process_mouse_action(float m_x, float m_y) {

	draw_selection();
	
	glReadBuffer(GL_BACK);
	unsigned char PixelColor[3];
	glReadPixels(m_x, m_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &PixelColor);
	std::cout << int(PixelColor[0]) << " "<< int(PixelColor[1]) << " "	<< int(PixelColor[2]) << std::endl;
	Terrian->select(PixelColor);
}

void world::init() {

	//the lighting shader for instanced objects
	lighting_in = new Shader("lighting_instance.vs", "lighting_instance.fs");

	Terrian = new terrian();
	Terrian->set_projection(projection);
	Terrian->set_cam(view);
	Terrian->set_cube_shader(lighting_in);
	Terrian->cubes_init();

	OBJM = new object_manger();
	OBJM->set_projection(projection);
	OBJM->set_cam(view);
	OBJM->set_standered_shader(lighting_in);
	OBJM->init();

	BM = new beast_manager();
	BM->set_projection(projection);
	BM->set_cam(view);
	BM->set_terrian(Terrian);
	BM->set_map_size(Terrian->get_x_width(), Terrian->get_z_width());
	BM->set_def_shader(lighting_in);
	BM->init();

	Sky = new sky();
	Sky->set_projection(projection);
	Sky->set_cam(view);
	Sky->set_width(Terrian->get_x_width(), Terrian->get_y_width(), Terrian->get_z_width());
	Sky->init();

	selection = new Shader("selection.vs", "selection.fs");

}

void world::init_lighting_test() {
	moon = new Model("resources/objects/planet/planet.obj");
	cube = new Model("resources/objects/cube/cube.obj");
	lighting = new Shader("lighting.vs", "lighting.fs");
	lighting_in = new Shader("lighting_instance.vs", "lighting_instance.fs");
	def = new Shader("planet.vs", "planet.fs");
}

void world::draw_lighitng_test() {
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

void world::update_lighting_test(float deltaTime) {
	x = 0 + 30 * cos(angle);
	y = -3 + 30 * sin(angle);

	angle += (angle_incr * deltaTime);
	if (angle >= 360) {
		angle = 0;
	}
}