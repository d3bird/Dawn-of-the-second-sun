#include "world.h"

world::world() {
	update_projection = false;
	update_cam = false;

	//lighting test
	x = 20.0f;
	y = -3.0f;
	z = 0.0f;
	angle = 0;
	gen_orders = NULL;
	Time = NULL;
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

	OBJM->set_cam(view);
	OBJM->draw();

}

void world::draw_selection() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Terrian->set_cam(view);
	Terrian->draw_selection(selection);
	glFlush();
}

void world::update() {

	//Terrian->update(deltaTime);
	Terrian->update_zones();

	if (gen_orders->size() > 0) {
		for (int i = 0; i < gen_orders->size();i++) {
			BM->create_tasks(gen_orders[0][i]);
		}
		gen_orders->clear();
	}

	Sky->update();
	BM->update();
	OBJM->update();

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

	if (Time != NULL) {
		deltatime = Time->get_time_change();
	}
	else {
		std::cout << "there was a problem getting time" << std::endl;
		while (true);
	}

	//the lighting shader for instanced objects
	lighting_in = new Shader("shaders/lighting_instance.vs", "shaders/lighting_instance.fs");
	selection = new Shader("shaders/selection.vs", "shaders/selection.fs");
	std::cout << std::endl;
	OBJM = new object_manger();
	OBJM->set_projection(projection);
	OBJM->set_cam(view);
	OBJM->set_standered_shader(lighting_in);
	OBJM->set_time(Time);
	OBJM->init();
	std::cout << std::endl;

	Terrian = new terrian();
	Terrian->set_projection(projection);
	Terrian->set_cam(view);
	Terrian->set_cube_shader(lighting_in);
	Terrian->set_object_manger(OBJM);
	Terrian->set_time(Time);
	Terrian->cubes_init();
	gen_orders = Terrian->get_gen_jobs_pointer();
	Terrian->print_map_zoned();
	//while (true);

	Terrian->spawn_item(LOG_T, 7, 0);
	Terrian->spawn_item(LOG_T, 8, 0);
	Terrian->spawn_item(LOG_T, 9, 0);
	//Terrian->spawn_item(LOG_T, 10, 0);

	Terrian->spawn_item(LOG_T, 10, 0);
	Terrian->spawn_item(LOG_T, 10, 1);
	Terrian->spawn_item(LOG_T, 10, 2);
	Terrian->spawn_item(LOG_T, 10, 3);

	std::cout << std::endl;

	BM = new beast_manager();
	BM->set_projection(projection);
	BM->set_cam(view);
	BM->set_terrian(Terrian);
	BM->set_map_size(Terrian->get_x_width(), Terrian->get_z_width());
	BM->set_def_shader(lighting_in);
	BM->set_object_manger(OBJM);
	BM->set_time(Time);
	BM->init();
	std::cout << std::endl;

	Sky = new sky();
	Sky->set_projection(projection);
	Sky->set_cam(view);
	Sky->set_width(Terrian->get_x_width(), Terrian->get_y_width(), Terrian->get_z_width());
	Sky->set_time(Time);
	Sky->init();
	std::cout << std::endl;

	EVM = new event_manager();
	EVM->set_projection(projection);
	EVM->set_cam(view);
	EVM->set_time(Time);
	EVM->init();

	//std::cout << "printing info" << std::endl;
	//Terrian->get_spawn_zone()->print_info();

	//add the creatures
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	BM->spawn_creature(Terrian->get_spawn_zone());
	std::cout << std::endl;


	//spawn the tasks
	work_order* new_task = Terrian->generate_work_order(SACRIFICE_OBJ, 0, 5, 0);
	//Terrian->print_work_order(new_task);
	BM->create_tasks(new_task);
	

	new_task = Terrian->generate_work_order(SACRIFICE_OBJ, 2, 5, 0);
	//Terrian->print_work_order(new_task);
	BM->create_tasks(new_task);


	new_task = Terrian->generate_work_order(MOVE_C, 4, 5, 4);
	//Terrian->print_work_order(new_task);
	BM->create_tasks(new_task);
	new_task = Terrian->generate_work_order(MOVE_C, 10, 5, 4);
	//Terrian->print_work_order(new_task);
	BM->create_tasks(new_task);

	//std::cout << std::endl;

	//because of the way that this function works, the box should always be one bigger TODO: fix this 
	//std::vector<work_order*> workOplaenty = Terrian->generate_work_order_m(STOCK_OBJ, 7, 5, 0, 11, 5, 0);//xline test
	//std::vector<work_order*> workOplaenty = Terrian->generate_work_order_m(STOCK_OBJ, 10, 5, 0, 10, 5, 4);//zline test
	std::vector<work_order*> workOplaenty = Terrian->generate_work_order_m(STOCK_OBJ, 7, 5, 0, 11, 5, 4);//box test


	for (int i = 0; i < workOplaenty.size(); i++) {
		BM->create_tasks(workOplaenty[i]);
	}

	//item stacks testing
	Terrian->print_map_blocked();
//	Sky->pause_time_at_noon();
	//while (true);
}

void world::init_lighting_test() {
	moon = new Model("resources/objects/planet/planet.obj");
	cube = new Model("resources/objects/cube/cube.obj");
	lighting = new Shader("shaders/lighting.vs", "shaders/lighting.fs");
	lighting_in = new Shader("shaders/lighting_instance.vs", "shaders/lighting_instance.fs");
	def = new Shader("shaders/planet.vs", "shaders/planet.fs");
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

void world::change_projection(glm::mat4 i) {
	projection = i;
	Sky->set_projection(projection);
	BM->set_projection(projection);
	OBJM->set_projection(projection);
	Terrian->set_projection(projection);
}