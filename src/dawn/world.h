#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "terrian.h"
#include "beast_manager.h"
#include "object_manger.h"
#include "event_manager.h"
#include "sky.h"
#include "skymap.h"
#include "time.h"

class world{
public:
	world();

	void draw();
	void draw_selection();

	void update();
	void process_mouse_action(float m_x, float m_y);
	void init();

	//settersand getters
	//before the components are inited
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i;  }
	void set_time(timing* i) { Time = i; }
	//after the componets are inited
	void change_projection(glm::mat4 i);

private:

	void init_lighting_test();
	void draw_lighitng_test();
	void update_lighting_test(float deltaTime);

	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

	timing* Time;
	float* deltatime;
	bool* draw_events;

	terrian* Terrian;
	beast_manager* BM;
	object_manger* OBJM;
	event_manager* EVM;
	sky* Sky;
	skymap* Skymap;

	//auto generated work orders
	std::vector< work_order*>* gen_orders;

	//lighting test
	Model* moon;
	Model* cube;
	Shader* lighting;//for one object
	Shader* def;//no lighting
	Shader* lighting_in;//lighting for instanced objects

	//light possitions 
	float x;
	float y;
	float z;
	float angle;
	float angle_incr = 0.3;

	//selection shader
	Shader* selection;
};

